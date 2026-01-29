# Cub3D Kod Revizyonu - 42 The Norm Uyumlu

## 🔹 Genel Sorunlar ve Çözümler

### Mevcut Kodlardaki Ana Norm İhlalleri:
1. **For döngüleri** - while ile değiştirilmeli
2. **Bir satırda birden fazla işlem** - ayrılmalı  
3. **Fonksiyon başında değişken tanımlanmaması** - düzeltilmeli
4. **25 satır sınırını aşan fonksiyonlar** - bölünmeli
5. **4'ten fazla parametre** - azaltılmalı
6. **80 karakter satır sınırı aşımları** - kısaltılmalı

---

## 📁 1. PLAYER.C DÜZELTMELERİ

### ❌ Sorun 1: `init_player` fonksiyonu - değişkenler başta tanımlanmamış
**Çözüm:** Tüm değişkenleri fonksiyon başında tanımla

### ❌ Sorun 2: `move_player` fonksiyonu - 25+ satır
**Çözüm:** Fonksiyonu 3 parçaya böl:
- `handle_rotation()`
- `handle_forward_backward()`  
- `handle_strafe()`

### ❌ Sorun 3: Yorumlanan kod blokları
**Çözüm:** Gereksiz yorumları temizle

### ❌ Sorun 4: Magic number kullanımı
**Çözüm:** Tanımları header'da define et

---

## 📁 2. MAIN.C DÜZELTMELERİ

### ❌ Sorun 1: `put_pixel` fonksiyonu - değişkenler başta tanımlanmamış
**Çözüm:** `index` değişkenini başta tanımla

### ❌ Sorun 2: `draw_square` fonksiyonu - for döngüleri
**Çözüm:** 4 ayrı while döngüsü kullan veya tek helper fonksiyon yaz

### ❌ Sorun 3: `draw_map` fonksiyonu - nested for döngüleri
**Çözüm:** while döngüleri ile değiştir

### ❌ Sorun 4: `clear_image` fonksiyonu - nested for döngüleri  
**Çözüm:** while döngüleri ile değiştir

### ❌ Sorun 5: `close_game` fonksiyonu - 25+ satır
**Çözüm:** Helper fonksiyonlara böl

### ❌ Sorun 6: Gereksiz include ve yorumlar
**Çözüm:** Temizle

---

## 📁 3. RAYCAST.C DÜZELTMELERİ

### ❌ Sorun 1: `touch` fonksiyonu - değişkenler başta tanımlanmamış
**Çözüm:** x, y değişkenlerini başta tanımla

### ❌ Sorun 2: `fixed_dist` fonksiyonu - değişkenler başta tanımlanmamış
**Çözüm:** Tüm değişkenleri başta tanımla

### ❌ Sorun 3: `draw_line` fonksiyonu - 25+ satır ve karmaşık
**Çözüm:** 2 fonksiyona böl:
- `cast_ray()` - ışın gönderme
- `draw_wall()` - duvar çizme

### ❌ Sorun 4: `perform_raycasting` fonksiyonu - for döngüsü
**Çözüm:** while döngüsü ile değiştir

---

## 🛠️ ADIM ADIM YAZIM SIRASI

### Sıra 1: Header Dosyasını Düzenle
- Magic number'ları define olarak ekle
- Missing function prototypes ekle

### Sıra 2: player.c - Baştan Yaz
1. `init_player()` - değişkenler başta
2. `handle_rotation()` - yeni helper fonksiyon
3. `handle_movement()` - yeni helper fonksiyon  
4. `handle_strafe()` - yeni helper fonksiyon
5. `move_player()` - helper'ları çağıran ana fonksiyon
6. `key_press()` ve `key_release()` - temiz versiyonlar

### Sıra 3: raycast.c - Baştan Yaz
1. `is_wall()` - touch yerine (daha açık isim)
2. `calculate_distance()` - distance yerine
3. `calculate_fixed_distance()` - fixed_dist yerine
4. `cast_single_ray()` - ray casting logic
5. `draw_wall_slice()` - wall rendering
6. `perform_raycasting()` - ana koordinatör

### Sıra 4: main.c - Baştan Yaz  
1. `put_pixel()` - düzenli versiyon
2. `draw_horizontal_line()` - helper fonksiyon
3. `draw_vertical_line()` - helper fonksiyon
4. `draw_square()` - helper'ları kullanarak
5. `draw_map()` - while döngüleri ile
6. `clear_image()` - while döngüleri ile
7. `cleanup_resources()` - close_game helper'ı
8. `init_game()` - düzenli versiyon
9. `draw_loop()` - ana loop
10. `close_game()` - temiz versiyon
11. `main()` - son versiyon

---

## ✅ Her Dosya İçin Kontrol Listesi

### Player.c Kontrolleri:
- [ ] Tüm değişkenler fonksiyon başında tanımlı mı?
- [ ] For döngüsü var mı? (olmamalı)
- [ ] Her fonksiyon 25 satırdan az mı?
- [ ] Her fonksiyon 4'ten az parametre alıyor mu?
- [ ] Magic number var mı? (olmamalı)

### Raycast.c Kontrolleri:
- [ ] Boolean return değerleri doğru mu? 
- [ ] Float hesaplamalar başta tanımlı mı?
- [ ] While döngüleri doğru çalışıyor mu?
- [ ] Fonksiyon isimleri açıklayıcı mı?

### Main.c Kontrolleri:
- [ ] MLX fonksiyon çağrıları doğru mu?
- [ ] Memory leak riski var mı?
- [ ] Error handling yeterli mi?
- [ ] Include'lar minimal mi?

---

## 📋 Yazım Öncelik Sırası:

1. **İlk**: Header dosyasındaki define'ları ekle
2. **İkinci**: player.c - en basit başlangıç
3. **Üçüncü**: raycast.c - core logic  
4. **Dördüncü**: main.c - everything together

## 💡 Önemli Notlar:
- Her fonksiyonu yazdıktan sonra compile et
- Test için DEBUG modunu kullan
- Bir seferde bir dosya yaz
- Her değişiklikten sonra git commit at
- Fonksiyon isimlerini açıklayıcı yap
