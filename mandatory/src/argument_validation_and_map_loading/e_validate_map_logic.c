#include "../../inc/so_long.h"
#include <stdio.h>
static void free_copied_grid(char** grid, int height)
{
    int i;

    if (!grid)
        return;
    i = 0;
    // height kadar satır ayrıldığı için height'a kadar dön
    while (i < height) {
        if (grid[i]) // Serbest bırakmadan önce pointer'ın geçerli olup olmadığını kontrol et
            free(grid[i]);
        grid[i] = NULL; // İyi pratik
        i++;
    }
    free(grid); // Pointer dizisini serbest bırak
}

// Flood fill analizi için harita grid'inin bir kopyasını oluşturur.
static char** copy_map_grid(t_map* map)
{
    char** grid_copy;
    int i;

    // height kadar satır pointer'ı + 1 NULL sonlandırıcı için yer ayır
    grid_copy = (char**)malloc(sizeof(char*) * (map->height + 1));
    if (!grid_copy)
        return (NULL);
    i = 0;
    while (i < map->height) {
        grid_copy[i] = ft_strdup(map->grid[i]); // Her satırı kopyala
        if (!grid_copy[i]) {
            free_copied_grid(grid_copy, i); // Hata durumunda kısmen ayrılanları temizle
            return (NULL);
        }
        i++;
    }
    grid_copy[i] = NULL; // Pointer dizisini NULL ile sonlandır
    return (grid_copy);
}

// Rekürsif flood fill algoritması. Ulaşılabilir yerleri ('0', 'C', 'E', 'P') 'F' ile işaretler.
// ÖNEMLİ: Duvar ('1') ve zaten doldurulmuş ('F') yerlerde durur. Çıkışa ('E') GİRER.
static void flood_fill(char** grid, int x, int y, int height, int width)
{
    // 1. Temel Durum: Sınırların Dışı
    //    Koordinatların geçerli aralıkta olup olmadığını KONTROL ET
    if (x < 0 || x >= height || y < 0 || y >= width)
        return;

    // 2. Temel Durum: Duvar veya Zaten Ziyaret Edilmiş
    //    grid[x][y] erişiminden ÖNCE grid[x]'in geçerli olup olmadığını kontrol et!
    if (!grid || !grid[x] || grid[x][y] == '1' || grid[x][y] == 'F')
        return;

    // Mevcut hücreyi ziyaret edildi ('F') olarak işaretle
    grid[x][y] = 'F';

    // Komşular için rekürsif çağrılar (Sınır kontrolü bir sonraki çağrıda yapılır)
    flood_fill(grid, x + 1, y, height, width); // Aşağı
    flood_fill(grid, x - 1, y, height, width); // Yukarı
    flood_fill(grid, x, y + 1, height, width); // Sağ
    flood_fill(grid, x, y - 1, height, width); // Sol
}

// Tüm koleksiyonların ('C') ve çıkışın ('E') flood fill ile erişilip erişilmediğini kontrol eder.
// Orijinal haritayı doldurulmuş kopya ile karşılaştırır.
static t_map_error check_accessibility(char** filled_grid, t_map* map)
{
    int x;
    int y;

    x = 0;
    while (x < map->height) {
        y = 0;
        // Doğrulanmış map->width kullanılır. Döngü genişliğe kadar gitmeli!
        while (y < map->width) {
            // Eğer orijinal haritada bir hedef ('C' veya 'E') varsa...
            if (map->grid[x][y] == 'C' || map->grid[x][y] == 'E') {
                // ...ama doldurulmuş kopyada bu konum 'F' değilse (ulaşılamamışsa)...
                // filled_grid[x]'in NULL olmadığını kontrol et!
                if (!filled_grid || !filled_grid[x] || filled_grid[x][y] != 'F') {
                    // Hata: Ulaşılamayan bir 'C' veya 'E' bulundu.
                    return (ERR_MAP_PATH);
                }
            }
            y++;
        }
        x++;
    }
    // Eğer döngü bittiyse ve hata dönülmediyse, tüm 'C' ve 'E'ler ulaşılabilirdir.
    return (MAP_SUCCESS);
}

// Harita mantığını flood fill kullanarak doğrular: koleksiyonlara ve çıkışa yol var mı?
t_map_error validate_map_logic(t_map* map)
{
    char** grid_copy; // Flood fill için değiştirilebilir kopya
    t_map_error status;

    // Oyuncu başlangıç koordinatlarının map struct'ında set edildiğini varsayıyoruz
    if (map->player_start_x < 0 || map->player_start_y < 0) // Basit bir kontrol
        return (ERR_MAP_NO_PLAYER); // Veya uygun bir hata

    // Flood fill için haritanın değiştirilebilir bir kopyasını oluştur
    grid_copy = copy_map_grid(map);
    if (!grid_copy)
        return (ERR_MALLOC); // Kopyalama sırasında hafıza hatası

    // Flood fill'i oyuncunun başlangıç konumundan başlat
    // (map->player_start_x, map->player_start_y önceden bulunmuş olmalı)
    flood_fill(grid_copy, map->player_start_x, map->player_start_y,
        map->height, map->width);

    // Orijinal haritadaki tüm 'C' ve 'E'lerin, kopyada 'F' olup olmadığını kontrol et
    status = check_accessibility(grid_copy, map);

    // Kopya grid'i sonuç ne olursa olsun serbest bırak
    free_copied_grid(grid_copy, map->height);

    // Erişilebilirlik kontrolünün sonucunu döndür
    return (status);
}
