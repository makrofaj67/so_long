#include "../inc/so_long.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> // srand, rand için

// --- Keycodes (Linux/X11 - MacOS'ta farklıdır!) ---
#define KEY_ESC 65307
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100

#define TILE_SIZE 64
#define WIN_WIDTH 16 * TILE_SIZE  // 1024
#define WIN_HEIGHT 12 * TILE_SIZE // 768
#define MOVE_SPEED TILE_SIZE / 8 // Daha akıcı hareket için küçük adımlar

#define MAX_HURDLES 50 // Maksimum engel sayısı (tahmini)

// --- İmaj Veri Yapısı ---
// mlx_get_data_addr'dan dönen bilgileri ve boyutları saklamak için
typedef struct s_img_data {
    void    *img_ptr;
    char    *addr;
    int     bpp;          // bits per pixel
    int     line_len;
    int     endian;
    int     width;
    int     height;
}   t_img_data;

// --- Engel Pozisyon Yapısı ---
typedef struct s_pos {
    int x;
    int y;
}   t_pos;

// --- mlx_entry Struct'ını Güncelle ---
typedef struct s_mlx_entry {
    void        *mlx;
    void        *win;

    // Arka Tampon (Frame Buffer)
    t_img_data  frame_buffer;

    // Yüklenmiş Sprite'lar (Görseller)
    t_img_data  head_img;
    t_img_data  wall_img;
    t_img_data  floor_img;
    t_img_data  hurdle_img;
    // t_img_data  grid_img; // Gerekliyse

    // Oyuncu (Kafa) Durumu
    int         head_x;
    int         head_y;

    // Engel Pozisyonları
    t_pos       hurdles[MAX_HURDLES];
    int         hurdle_count;

}   mlx_entry;

// --- Yardımcı: Bir imajın pikselini alma ---
// Dikkat: Basitlik için şeffaflık kontrolü eklenmedi.
unsigned int get_pixel_color(t_img_data *img, int x, int y)
{
    char    *dst;

    if (x < 0 || x >= img->width || y < 0 || y >= img->height)
        return (0x00000000); // Sınır dışıysa siyah (veya şeffaf renk)
    dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
    // Küçük endian varsayımıyla doğrudan int olarak okuyoruz (çoğu sistem)
    return (*(unsigned int*)dst);
}

// --- Yardımcı: Arka tampona bir piksel koyma ---
void put_pixel_to_buffer(t_img_data *buffer, int x, int y, unsigned int color)
{
    char    *dst;

    // Sınır kontrolü
    if (x < 0 || x >= buffer->width || y < 0 || y >= buffer->height)
        return;

    dst = buffer->addr + (y * buffer->line_len + x * (buffer->bpp / 8));
    *(unsigned int*)dst = color;
}

// --- Yardımcı: Bir sprite'ı arka tampona kopyalama ---
// Basit opak kopyalama yapar. Şeffaflık için geliştirilebilir.
void copy_sprite_to_buffer(t_img_data *buffer, t_img_data *sprite, int target_x, int target_y)
{
    int x, y;
    unsigned int color;

    for (y = 0; y < sprite->height; ++y)
    {
        for (x = 0; x < sprite->width; ++x)
        {
            color = get_pixel_color(sprite, x, y);
            // İsteğe bağlı: Şeffaflık kontrolü
            // if (color == 0xFF000000) // Örn: Tam siyah alfa=0 ise (veya XPM'in şeffaf rengi)
            //     continue;
            put_pixel_to_buffer(buffer, target_x + x, target_y + y, color);
        }
    }
}

// --- Ortam Çizim Fonksiyonları (Arka Tampona Çizer) ---

void draw_hurdles_to_buffer(mlx_entry* pax)
{
    int i;
    for (i = 0; i < pax->hurdle_count; ++i) {
        copy_sprite_to_buffer(&pax->frame_buffer, &pax->hurdle_img, pax->hurdles[i].x, pax->hurdles[i].y);
    }
}

void draw_walls_to_buffer(mlx_entry* pax)
{
    int x, y;
    for (x = 0; x < WIN_WIDTH / TILE_SIZE; ++x) {
        for (y = 0; y < WIN_HEIGHT / TILE_SIZE; ++y) {
            if (x == 0 || x == (WIN_WIDTH / TILE_SIZE) - 1 || y == 0 || y == (WIN_HEIGHT / TILE_SIZE) - 1) {
                 copy_sprite_to_buffer(&pax->frame_buffer, &pax->wall_img, x * TILE_SIZE, y * TILE_SIZE);
            }
        }
    }
}

void draw_floor_to_buffer(mlx_entry* pax)
{
    int x, y;
    x = 1;
    while (x < (WIN_WIDTH / TILE_SIZE) - 1) {
        y = 1;
        while (y < (WIN_HEIGHT / TILE_SIZE) - 1) {
             copy_sprite_to_buffer(&pax->frame_buffer, &pax->floor_img, x * TILE_SIZE, y * TILE_SIZE);
            y++;
        }
        x++;
    }
}

// --- Tüm Sahneyi Arka Tampona Çizen ve Ekrana Kopyalayan Ana Fonksiyon ---
int render_frame(void *param)
{
    mlx_entry* pax = (mlx_entry*)param;

    // --- Arka Tampona Çizim ---
    // 1. Zemini çiz
    draw_floor_to_buffer(pax);
    // 2. Duvarları çiz
    draw_walls_to_buffer(pax);
    // 3. Engelleri çiz (önceden belirlenmiş pozisyonlardan)
    draw_hurdles_to_buffer(pax);
    // 4. Hareketli Nesneyi (Kafa) çiz
    copy_sprite_to_buffer(&pax->frame_buffer, &pax->head_img, pax->head_x, pax->head_y);

    // --- Arka Tamponu Pencereye Kopyala ---
    // Tüm çizimler bittikten sonra, tek seferde pencereye aktarılır.
    mlx_put_image_to_window(pax->mlx, pax->win, pax->frame_buffer.img_ptr, 0, 0);

    // mlx_clear_window() artık GEREKLİ DEĞİLDİR, çünkü buffer her şeyi kaplar.
    // Nadiren bazı sistemlerde temizlik sonrası put_image daha iyi olabilir,
    // ama genellikle çift tamponlamanın amacı clear'ı gereksiz kılmaktır.

    return (0);
}

// --- Tuş Basma İşleyici (Hareketi Günceller - Değişiklik Yok) ---
int kafa_move(int keycode, void* param)
{
    mlx_entry* pax = (mlx_entry*)param;
    int next_x = pax->head_x;
    int next_y = pax->head_y;

    // printf("Key pressed: %d\n", keycode); // Debug

    if (keycode == KEY_ESC)
    {
        // --- Temizlik (Arka tamponu da ekle) ---
        mlx_destroy_image(pax->mlx, pax->frame_buffer.img_ptr); // ÖNEMLİ
        mlx_destroy_image(pax->mlx, pax->head_img.img_ptr);
        mlx_destroy_image(pax->mlx, pax->wall_img.img_ptr);
        mlx_destroy_image(pax->mlx, pax->floor_img.img_ptr);
        mlx_destroy_image(pax->mlx, pax->hurdle_img.img_ptr);
        mlx_destroy_window(pax->mlx, pax->win);
        // mlx_destroy_display(pax->mlx); // Linux'ta bazen gerekli
        free(pax->mlx); // mlx_init için ayrılanı serbest bırak (bazı sistemlerde)
        printf("Exiting gracefully.\n");
        exit(0); // Programdan çık
    }
    else if (keycode == KEY_W) next_y -= MOVE_SPEED;
    else if (keycode == KEY_S) next_y += MOVE_SPEED;
    else if (keycode == KEY_A) next_x -= MOVE_SPEED;
    else if (keycode == KEY_D) next_x += MOVE_SPEED;
    else
        return (0); // Başka tuşsa bir şey yapma

    // --- Çarpışma Kontrolü (Basit Sınır Kontrolü) ---
    if (next_x < TILE_SIZE) next_x = TILE_SIZE;
    if (next_y < TILE_SIZE) next_y = TILE_SIZE;
    if (next_x + pax->head_img.width > WIN_WIDTH - TILE_SIZE) next_x = WIN_WIDTH - TILE_SIZE - pax->head_img.width;
    if (next_y + pax->head_img.height > WIN_HEIGHT - TILE_SIZE) next_y = WIN_HEIGHT - TILE_SIZE - pax->head_img.height;

    pax->head_x = next_x;
    pax->head_y = next_y;

    return (0);
}

// --- Görselleri Yükleyen ve Veri Adresini Alan Yardımcı Fonksiyon ---
int load_image_data(mlx_entry *pax, t_img_data *img_data, char *path)
{
    img_data->img_ptr = mlx_xpm_file_to_image(pax->mlx, path, &img_data->width, &img_data->height);
    if (!img_data->img_ptr) {
        printf("Error: Could not load image %s\n", path);
        return (0); // Başarısız
    }
    img_data->addr = mlx_get_data_addr(img_data->img_ptr, &img_data->bpp, &img_data->line_len, &img_data->endian);
    if (!img_data->addr) {
         printf("Error: Could not get data address for %s\n", path);
         mlx_destroy_image(pax->mlx, img_data->img_ptr); // Yüklenen imajı temizle
         img_data->img_ptr = NULL;
         return (0); // Başarısız
    }
    printf("Loaded: %s (%dx%d, %d bpp)\n", path, img_data->width, img_data->height, img_data->bpp);
    return (1); // Başarılı
}

// --- Başlangıçta Engel Pozisyonlarını Belirleme ---
void initialize_hurdles(mlx_entry *pax)
{
    int x, y;
    float random_value;
    pax->hurdle_count = 0;
    srand(time(NULL)); // Rastgele sayı üretecini başlat

    for (x = 1; x < (WIN_WIDTH / TILE_SIZE) - 1; ++x) {
        for (y = 1; y < (WIN_HEIGHT / TILE_SIZE) - 1; ++y) {
            // Başlangıç kafa pozisyonuna engel koyma
            if (x == pax->head_x / TILE_SIZE && y == pax->head_y / TILE_SIZE)
                continue;

            random_value = (float)rand() / (float)RAND_MAX;
            if (random_value > 0.9 && pax->hurdle_count < MAX_HURDLES) {
                pax->hurdles[pax->hurdle_count].x = x * TILE_SIZE;
                pax->hurdles[pax->hurdle_count].y = y * TILE_SIZE;
                pax->hurdle_count++;
            }
        }
    }
     printf("Initialized %d hurdles.\n", pax->hurdle_count);
}

// --- Kaynakları Temizleme Fonksiyonu (Hata veya Çıkış İçin) ---
void cleanup_resources(mlx_entry *pax)
{
    if (!pax) return;
    // İmajları yok et (NULL kontrolü yaparak)
    if (pax->frame_buffer.img_ptr) mlx_destroy_image(pax->mlx, pax->frame_buffer.img_ptr);
    if (pax->head_img.img_ptr) mlx_destroy_image(pax->mlx, pax->head_img.img_ptr);
    if (pax->wall_img.img_ptr) mlx_destroy_image(pax->mlx, pax->wall_img.img_ptr);
    if (pax->floor_img.img_ptr) mlx_destroy_image(pax->mlx, pax->floor_img.img_ptr);
    if (pax->hurdle_img.img_ptr) mlx_destroy_image(pax->mlx, pax->hurdle_img.img_ptr);

    // Pencereyi yok et
    if (pax->win) mlx_destroy_window(pax->mlx, pax->win);

    // MLX display bağlantısını yok et (Linux'ta)
    #ifdef __linux__
    // Bazen mlx_destroy_display gerekebilir, ancak free(pax->mlx) genellikle yeterlidir
    // mlx_destroy_display(pax->mlx);
    #endif

    // mlx_init tarafından ayrılan belleği serbest bırak (bazı sistemler/versiyonlar)
    if (pax->mlx) free(pax->mlx);

    printf("Resources cleaned up.\n");
}

// Pencere kapatma tuşu için hook (opsiyonel ama iyi fikir)
int close_window_hook(mlx_entry *pax)
{
	printf("Close button pressed. Exiting...\n");
    // ESC'ye basılmış gibi temizlik yap ve çık
	cleanup_resources(pax);
	exit(0);
	return (0); // Asla buraya ulaşmaz ama derleyici uyarısını engeller
}

// --- Oyun Başlatma ve Ana Döngü ---
int main(void)
{
    mlx_entry pax = {0}; // Struct'ı sıfırla başlatmak iyi bir pratiktir

    // MLX Başlat
    pax.mlx = mlx_init();
    if (!pax.mlx) { printf("Error: mlx_init failed.\n"); return (1); }

    // Pencere Oluştur
    pax.win = mlx_new_window(pax.mlx, WIN_WIDTH, WIN_HEIGHT, "So Long Double Buffer");
    if (!pax.win) { printf("Error: mlx_new_window failed.\n"); free(pax.mlx); return (1); }

    // --- Arka Tamponu Oluştur ---
    pax.frame_buffer.img_ptr = mlx_new_image(pax.mlx, WIN_WIDTH, WIN_HEIGHT);
    if (!pax.frame_buffer.img_ptr) {
        printf("Error: mlx_new_image for frame buffer failed.\n");
        cleanup_resources(&pax); return (1);
    }
    pax.frame_buffer.addr = mlx_get_data_addr(pax.frame_buffer.img_ptr, &pax.frame_buffer.bpp,
                                            &pax.frame_buffer.line_len, &pax.frame_buffer.endian);
    pax.frame_buffer.width = WIN_WIDTH; // Boyutları da sakla
    pax.frame_buffer.height = WIN_HEIGHT;
    if (!pax.frame_buffer.addr) {
         printf("Error: mlx_get_data_addr for frame buffer failed.\n");
         cleanup_resources(&pax); return (1);
    }
     printf("Frame buffer created (%dx%d, %d bpp)\n", pax.frame_buffer.width, pax.frame_buffer.height, pax.frame_buffer.bpp);


    // --- Sprite Görsellerini Yükle ---
    // Her yükleme sonrası hata kontrolü önemli!
    if (!load_image_data(&pax, &pax.head_img, "/home/luka/Desktop/so_long/rendu/mandatory/images/head.xpm") ||
        !load_image_data(&pax, &pax.wall_img, "/home/luka/Desktop/so_long/rendu/mandatory/images/blocks.xpm") ||
        !load_image_data(&pax, &pax.floor_img, "/home/luka/Desktop/so_long/rendu/mandatory/images/dirt.xpm") ||
        !load_image_data(&pax, &pax.hurdle_img, "/home/luka/Desktop/so_long/rendu/mandatory/images/metalic.xpm"))
    {
        cleanup_resources(&pax); // Hata durumunda temizlik yap
        return (1);
    }

    // --- Kafa Başlangıç Konumunu Ayarla ---
    pax.head_x = 8 * TILE_SIZE;
    pax.head_y = 6 * TILE_SIZE;

    // --- Engel Pozisyonlarını Başlat ---
    initialize_hurdles(&pax);

    // --- Hook'ları Ayarla ---
    mlx_hook(pax.win, 02, 1L << 0, kafa_move, &pax); // KeyPress
    mlx_hook(pax.win, 17, 0, close_window_hook, &pax); // DestroyNotify (Pencere Kapatma)
    mlx_loop_hook(pax.mlx, render_frame, &pax); // Ana çizim döngüsü

    printf("Starting main loop...\n");
    mlx_loop(pax.mlx); // Olay döngüsünü başlat

    // Normalde mlx_loop'tan çıkılmaz, çıkış hook içinde yapılır.
    // Eğer bir şekilde çıkılırsa diye yine de temizlik çağrılabilir.
    printf("mlx_loop finished (unexpected).\n");
    cleanup_resources(&pax);
    return (0);
}
