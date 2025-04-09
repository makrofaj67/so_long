/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_structs.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 05:46:36 by rakman            #+#    #+#             */
/*   Updated: 2025/04/09 15:57:13 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_STRUCTS_H
#define SO_STRUCTS_H

typedef enum e_map_error {
    MAP_SUCCESS = 0,     // 0: Başarı (Hata yok)

    // --- 1. Argüman ve Dosya Adı Kontrolleri ---
    ERR_ARGS_COUNT,      // 1: Yanlış sayıda argüman verildi.
    ERR_ARGS_EXT,        // 2: Dosya adı '.ber' uzantılı değil.

    // --- 2. Dosya Erişimi ve Temel Okuma Kontrolleri ---
    ERR_FILE_OPEN,       // 3: Dosya açılamadı (yok veya izin yok).
    ERR_MAP_EMPTY,       // 4: Dosya açılabildi ama içi boş.
    ERR_FILE_READ,       // 5: Dosya okunurken bir G/Ç hatası oluştu.

    // --- 3. Harita Yapısı ve İçerik Kontrolleri (Genellikle okuma sırasında/sonrasında) ---
    ERR_MAP_RECT,        // 6: Harita dikdörtgen değil (farklı satır uzunlukları).
    ERR_MAP_CHARS,       // 7: Haritada geçersiz karakter(ler) bulundu (0,1,P,E,C dışında).
    ERR_MAP_WALLS,       // 8: Harita tamamen duvarlarla ('1') çevrili değil.
    ERR_MAP_PLAYER,      // 9: Haritada tam olarak 1 tane 'P' yok (0 veya >1).
    ERR_MAP_EXIT,        // 10: Haritada tam olarak 1 tane 'E' yok (0 veya >1).
    ERR_MAP_COLLECT,     // 11: Haritada en az 1 tane 'C' yok (0 tane var).

    // --- 4. Harita Mantığı Kontrolü ---
    ERR_MAP_PATH,        // 12: Geçerli bir yol yok (P'den tüm C'lere ve sonra E'ye ulaşılamıyor).

    // --- 5. Genel Hatalar ---
    ERR_MALLOC           // 13: Bellek ayırma (malloc) başarısız oldu (herhangi bir aşamada olabilir).

} t_map_error;



typedef struct s_map {
	char **grid;
	int width;
	int height;
	int player_start_x;
	int player_start_y;
	int collectible_total;
	int exit_x;
	int exit_y;
} t_map;

typedef struct s_game {
	t_map map;
} t_game;

#endif
