# CUB3D YAPILACAKLAR LİSTESİ

## 🎯 MEVCUT DURUM ANALİZİ

### ✅ TAMAMLANMIŞ ÖZELLIKLER
- [x] Temel MLX pencere açma ve kapama
- [x] Oyuncu hareket kontrolü (WASD)
- [x] Görüş açısı döndürme (Sol/Sağ ok tuşları)
- [x] Temel raycasting algoritması
- [x] Duvar algılama (touch fonksiyonu)
- [x] Basit harita sistemi (hardcoded)
- [x] ESC ile çıkış yapma
- [x] X tuşu ile pencere kapatma

### 🔧 TEMEL SORUNLAR VE DÜZELTİLMESİ GEREKENLER

#### **1. 42 Norm Uyumluluk Sorunları**
- **`for` döngüleri yasak** - `while` döngülerine çevrilmeli
- **Değişken tanımları** - Fonksiyon başında tanımlanmalı (5 adet max)
- **Fonksiyon uzunlukları** - 25 satırı geçenler bölünmeli
- **Satır uzunlukları** - 80 karakteri geçenler kısaltılmalı
- **Global değişkenler** - Tamamen kaldırılmalı

#### **2. Mandatory Özellikler (Eksik)**

##### **A. Dosya Parseri (.cub)**
- [ ] `.cub` dosyası okuma sistemi
- [ ] Texture path'lerini okuma (NO, SO, WE, EA)
- [ ] Renk değerlerini okuma (F, C)
- [ ] Harita validation sistemi
- [ ] Hata yönetimi ("Error\n" + açıklama)

##### **B. Texture Sistemi**  
- [ ] Duvar texture'ları yükleme
- [ ] Yön bazlı texture seçimi (N/S/E/W)
- [ ] Texture mapping algoritması
- [ ] XPM dosya desteği

##### **C. Renk Sistemi**
- [ ] Tavan rengi uygulanması
- [ ] Zemin rengi uygulanması
- [ ] RGB renk parseri

##### **D. Harita Sistemi**
- [ ] Dinamik harita boyutu
- [ ] Harita validasyonu (duvarlarla çevrili)
- [ ] Oyuncu başlangıç pozisyonu (N/S/E/W)
- [ ] Boşluk karakteri desteği

##### **E. İyileştirilmesi Gerekenler**
- [ ] Fish-eye efekti düzeltmesi (fixed_dist fonksiyonu)
- [ ] Duvar çarpışma sistemi
- [ ] Piksel put optimizasyonu
- [ ] Memory leak kontrolü

---

## 📋 ÖNCELIK SIRASINA GÖRE YAPILACAKLAR

### **YÜKSEK ÖNCELİK (Mandatory)**

#### **1. 42 Norm Uyumluluk (ACİL)**
```c
// ÖRNEKLENDİRME GEREKLİ:
// draw_square, draw_map, clear_image fonksiyonlarındaki for döngüleri
// move_player, draw_line, perform_raycasting fonksiyonları çok uzun
```

#### **2. .cub Dosya Parseri**
- `parser.c` dosyasını implement etmek
- Dosya okuma fonksiyonları
- Texture path validasyonu
- Renk değeri validasyonu
- Harita parseri

#### **3. Texture Loading Sistemi**
- MLX XPM yükleme
- Texture struct tanımı
- Texture mapping algoritması

#### **4. Argument Handling**
```c
int main(int argc, char **argv)
{
    // .cub dosyası kontrolü
    // Argument validasyonu
}
```

### **ORTA ÖNCELİK**

#### **5. İyileştirilmiş Hareket Sistemi**
- Duvar çarpışma kontrolü
- Açı bazlı hareket (cos/sin kullanarak)
- Hareket smoothing

#### **6. Renk ve Render İyileştirmeleri**
- Tavan/zemin renk uygulaması
- Texture rendering
- Performance optimizasyonu

### **DÜŞÜK ÖNCELİK (Bonus)**

#### **7. Bonus Özellikler**
- Duvar çarpışmaları
- Minimap sistemi
- Açılır-kapanır kapılar
- Animated sprite'lar
- Mouse ile görüş döndürme

---

## 🔍 DETAYLI GÖREV PLANI

### **ADIM 1: Norm Uyumluluk**
```
Süre: 1-2 gün
Dosyalar: Tüm .c dosyaları
Önem: KRİTİK
```

### **ADIM 2: Parser Sistemi**
```
Süre: 2-3 gün  
Dosyalar: parser.c, utils.c
Fonksiyonlar:
- read_file()
- parse_texture()
- parse_color()
- parse_map()
- validate_map()
```

### **ADIM 3: Texture Sistemi**
```
Süre: 2-3 gün
Dosyalar: main.c, game.h
Fonksiyonlar:
- load_textures()
- get_texture_pixel()
- apply_texture()
```

### **ADIM 4: Test ve Debug**
```
Süre: 1-2 gün
- Valgrind ile memory leak kontrolü
- Farklı .cub dosyaları test etme
- Error handling test
```

---

## 📁 DOSYA YAPISI ÖNERİSİ

```
inc/
├── game.h          // Ana header dosyası
└── parser.h        // Parser fonksiyon tanımları

srcs/
├── main.c          // MLX ve ana döngü
├── parser.c        // .cub dosya okuma/parsing
├── texture.c       // Texture loading ve mapping  
├── player.c        // Oyuncu hareketleri
├── raycast.c       // Raycasting algoritması
├── render.c        // Rendering ve pixel operations
└── utils.c         // Yardımcı fonksiyonlar

maps/
├── simple.cub      // Test haritası
├── complex.cub     // Karmaşık test haritası
└── invalid.cub     // Hata test haritası

textures/
├── north.xpm       // Kuzey duvar texture
├── south.xpm       // Güney duvar texture  
├── east.xpm        // Doğu duvar texture
└── west.xpm        // Batı duvar texture
```

---

## ⚠️ KRİTİK UYARILAR

1. **42 Norm kontrolü her commit'te yapılmalı**
2. **Memory leak'ler valgrind ile kontrol edilmeli**
3. **Error handling tüm fonksiyonlarda implement edilmeli**
4. **Subject dosyasındaki tüm gereksinimler karşılanmalı**
5. **Test dosyaları hazırlanmalı**

---

## 🎯 BEKLENİLEN SONUÇ

Mandatory part tamamlandığında:
- .cub dosyası ile harita yüklenebilecek
- Texture'lı duvarlar görünecek  
- Renklı tavan/zemin olacak
- WASD ile hareket edilebilecek
- Ok tuşları ile bakış döndürülebilecek
- ESC/X ile çıkış yapılabilecek
- Tüm hata durumları handle edilecek
- 42 Norm'a uygun kod olacak

Bu listeye göre çalışıldığında proje başarılı bir şekilde tamamlanacaktır.
