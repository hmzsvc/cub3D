# Raycasting: Pixel-Step vs DDA - Detaylı Teknik Analiz

## 📋 İçindekiler
1. [Raycasting Temelleri](#1-raycasting-temelleri)
2. [Mevcut Kodunuzun (Pixel-Step) Analizi](#2-mevcut-kodunuzun-pixel-step-analizi)
3. [Problem: Texture Neden Bozuluyor?](#3-problem-texture-neden-bozuluyor)
4. [DDA Algoritması Açıklaması](#4-dda-algoritması-açıklaması)
5. [Kod Karşılaştırması](#5-kod-karşılaştırması)
6. [Wall Face (Duvar Yüzü) Tespiti](#6-wall-face-duvar-yüzü-tespiti)
7. [Texture Koordinat Hesabı](#7-texture-koordinat-hesabı)
8. [Performans Analizi](#8-performans-analizi)
9. [Özet ve Sonuç](#9-özet-ve-sonuç)

---

## 1. Raycasting Temelleri

### Raycasting Nedir?

Raycasting, 2D bir haritadan 3D görünüm elde etmek için kullanılan bir tekniktir. Wolfenstein 3D (1992) bu tekniği ünlü hale getirmiştir.

**Temel Mantık:**
```
Oyuncu → Her piksel için bir ışın (ray) gönder
         ↓
      Işın duvara çarpana kadar ilerle
         ↓
      Mesafeyi hesapla
         ↓
      Mesafeye göre duvar yüksekliğini belirle
         ↓
      Texture'ı bu yüksekliğe göre çiz
```

### Koordinat Sistemi

```
Y (Yukarı)
^
|
|    [P] ← Oyuncu
|     ↘ (Ray, 45° açıyla)
|       ↘
|         ↘
|           [1] ← Duvar
+----------------> X (Sağa)
```

**Önemli Kavramlar:**
- **Grid (Izgara):** Harita, BLOCK x BLOCK boyutundaki karelere bölünmüş (sizde BLOCK=64)
- **Ray (Işın):** Oyuncudan belirli bir açıyla gönderilen doğru
- **Hit Point (Çarpma Noktası):** Ray'in duvara çarptığı nokta
- **Wall Face (Duvar Yüzü):** Duvarın hangi tarafı (NORTH/SOUTH/EAST/WEST)

---

## 2. Mevcut Kodunuzun (Pixel-Step) Analizi

### Kodunuzun Çalışma Mantığı

Mevcut `cast_ray_data()` fonksiyonunuz şu şekilde çalışıyor:

```c
static void	cast_ray_data(t_game *g, float angle, float *ray_info)
{
    float	ray_x;
    float	ray_y;
    float	step_x;
    float	step_y;
    float	prev_x;
    float	prev_y;

    ray_x = g->player.x;      // Başlangıç: Oyuncunun konumu
    ray_y = g->player.y;
    step_x = cos(angle);      // X yönünde adım boyutu
    step_y = sin(angle);      // Y yönünde adım boyutu
    
    while (!is_wall_hit(ray_x, ray_y, g))
    {
        prev_x = ray_x;       // Önceki pozisyonu kaydet
        prev_y = ray_y;
        ray_x += step_x;      // ⚠️ HER ADIMDA 1 PİKSEL İLERLE
        ray_y += step_y;
    }
    
    // Duvara çarpmadan önceki son pozisyonu kullan
    ray_info[0] = prev_x;
    ray_info[1] = prev_y;
    // ...
}
```

### Adım Adım Örnek

**Senaryo:** Oyuncu (128, 128) konumunda, 45° açıyla ray gönderiyor.

```
step_x = cos(45°) = 0.707
step_y = sin(45°) = 0.707
```

**İlerleme:**
```
Adım 0:  ray_x = 128.000,  ray_y = 128.000  → Duvar yok
Adım 1:  ray_x = 128.707,  ray_y = 128.707  → Duvar yok
Adım 2:  ray_x = 129.414,  ray_y = 129.414  → Duvar yok
Adım 3:  ray_x = 130.121,  ray_y = 130.121  → Duvar yok
...
Adım 87: ray_x = 189.309,  ray_y = 189.309  → Duvar yok
Adım 88: ray_x = 190.016,  ray_y = 190.016  → Duvar yok
Adım 89: ray_x = 190.723,  ray_y = 190.723  → DUVAR! (192,192'de duvar var)
```

**Sorun Ne?**
- 89 adım atmış ama aslında sadece 1 grid hücresi (128→192 arası) geçmiş
- `prev_x=190.016` kullanılıyor ama **gerçek kesişim noktası 192.000 olmalıydı**
- Her adımda `is_wall_hit()` çağrılıyor → Performans kaybı

---

## 3. Problem: Texture Neden Bozuluyor?

### Problem 1: Aşırı Örnekleme (Over-sampling)

**Görselleştirme (Grid hücresi görünümü):**
```
Grid Hücre 2 (X: 128-192)
┌─────────────────────────────────────────┐
│                                         │
│  • • • • • • • • • • • • • • • • • •    │ ← 18 adet ray örneği!
│                                         │
│                                         │
└─────────────────────────────────────────┘

Grid Hücre 3 (X: 192-256) [DUVAR]
┌─────────────────────────────────────────┐
│ •••••                                   │ ← 5 adet örnek (duvarın içinde!)
│                                         │
│                                         │
└─────────────────────────────────────────┘
```

**Sonuç:** Aynı hücre için farklı `tex_x` değerleri hesaplanıyor çünkü:
- Ray 1: `prev_x = 189.5` → `tex_x = 25`
- Ray 2: `prev_x = 190.2` → `tex_x = 26`
- Ray 3: `prev_x = 190.9` → `tex_x = 27`

Aslında **aynı duvar yüzü** ama farklı texture koordinatları → Şeritlenme!

### Problem 2: Wall Face Belirsizliği

`get_wall_face()` fonksiyonunuz:

```c
static int	get_wall_face(float hit_x, float hit_y, float step_x, float step_y)
{
    float	dx;
    float	dy;

    dx = fmod(hit_x, BLOCK);  // Blok içindeki X pozisyonu (0-64)
    dy = fmod(hit_y, BLOCK);  // Blok içindeki Y pozisyonu (0-64)
    
    // Negatif değerleri düzelt
    if (dx < 0)
        dx += BLOCK;
    if (dy < 0)
        dy += BLOCK;
    
    // Hangi kenara yakınsa o yüzü seç
    if (dx < 1.0)           // Sol kenara çok yakın → WEST
        return (1);
    if (dx > BLOCK - 1.0)   // Sağ kenara çok yakın → EAST
        return (0);
    if (dy < 1.0)           // Üst kenara çok yakın → NORTH
        return (3);
    if (dy > BLOCK - 1.0)   // Alt kenara çok yakın → SOUTH
        return (2);
    
    // Ortadaysa ray yönüne bak
    if (fabs(step_x) > fabs(step_y))
        return (step_x > 0 ? 0 : 1);
    return (step_y > 0 ? 2 : 3);
}
```

**Problem:**
- `dx` ve `dy` değerleri **ray'in durduğu noktaya** göre hesaplanıyor
- Ama ray duvarın **içine girmiş** durumda
- "Ortadaysa ray yönüne bak" kısmı **tahmine** dayalı

**Örnek Hata:**
```
Senaryo: 45° açıyla ray gönderiyorsunuz

Ray gerçekte Y grid çizgisine (yatay duvar) çarpıyor
Ama fmod(hit_x, BLOCK) = 32 çıkıyor (tam ortada)
     fmod(hit_y, BLOCK) = 2 çıkıyor (kenara yakın)

Kod "dy < 1.0" olmadığı için "ortadaysa" dalına giriyor
Ve step_x > step_y kontrolü yapıyor
→ EAST/WEST seçiyor (YANLIŞ!)
Gerçekte NORTH/SOUTH olmalıydı
```

### Problem 3: Texture X Koordinatı Tutarsızlığı

`calc_tex_x()` fonksiyonunuz:

```c
static int	calc_tex_x(float hit_x, float hit_y, t_texture *tex, int face)
{
    float	wall_hit;
    int		tex_x;

    if (face == 0 || face == 1)  // EAST veya WEST
        wall_hit = fmod(hit_y, BLOCK);  // Y koordinatını kullan
    else                          // NORTH veya SOUTH
        wall_hit = fmod(hit_x, BLOCK);  // X koordinatını kullan
    
    if (wall_hit < 0)
        wall_hit += BLOCK;
    
    wall_hit = wall_hit / BLOCK;  // 0.0 - 1.0 arasına normalize et
    tex_x = (int)(wall_hit * tex->width);
    
    if (tex_x < 0)
        tex_x = 0;
    if (tex_x >= tex->width)
        tex_x = tex->width - 1;
    
    return (tex_x);
}
```

**Problem:**
- `hit_x` ve `hit_y` **yaklaşık** değerler (prev_x, prev_y)
- Aynı duvar için farklı ray'ler farklı `hit_y` değerleri veriyor
- Sonuç: Texture koordinatları atlıyor veya tekrarlıyor

**Matematiksel Örnek:**
```
Duvar pozisyonu: X=192 (sabit)
Texture genişliği: 64 pixel

Ray 1: hit_y = 189.5
       wall_hit = fmod(189.5, 64) = 61.5
       wall_hit / BLOCK = 61.5 / 64 = 0.9609
       tex_x = 0.9609 * 64 = 61

Ray 2: hit_y = 190.2
       wall_hit = fmod(190.2, 64) = 62.2
       wall_hit / BLOCK = 62.2 / 64 = 0.9718
       tex_x = 0.9718 * 64 = 62

Ray 3: hit_y = 190.9
       wall_hit = fmod(190.9, 64) = 62.9
       wall_hit / BLOCK = 62.9 / 64 = 0.9828
       tex_x = 0.9828 * 64 = 62
```

**Sorun:** Aynı duvar bloğu için tex_x değerleri 61→62→62 şeklinde atlıyor!

---

## 4. DDA Algoritması Açıklaması

### DDA Nedir?

**DDA (Digital Differential Analysis)** = Dijital Diferansiyel Analiz

**Ana Fikir:** Pixel-pixel ilerlemek yerine **grid çizgilerinden çizgiye atla**!

### Matematiksel Temel

**Soru:** Bir ray grid çizgilerine ne zaman çarpar?

```
Grid yapısı (BLOCK = 64):

    0   64  128 192 256
    |   |   |   |   |
    +---+---+---+---+
    |   |   |   |   |
    +---+---+---+---+
    |   | P→→→|   |   | ← Ray yatay ve dikey çizgilere çarpıyor
    +---+---+---+---+
```

**DDA Formülü:**

Ray'in bir sonraki **dikey grid çizgisine** (X sabitlenmiş) kadar gitmesi gereken mesafe:
```
delta_x = |1.0 / cos(angle)|
```

Ray'in bir sonraki **yatay grid çizgisine** (Y sabitlenmiş) kadar gitmesi gereken mesafe:
```
delta_y = |1.0 / sin(angle)|
```

**Neden Bu Formül?**

Trigonometri:
```
tan(angle) = y / x
→ Eğer x'te 1 birim (grid hücresi) ilerlersek, y'de tan(angle) kadar ilerleriz
→ Toplam mesafe: sqrt(1² + tan(angle)²) = sqrt(1 + sin²/cos²) = 1/cos(angle)
```

### DDA Adımları

**1. Başlangıç Hesaplamaları**

```c
// Oyuncu grid hücresinde nerede?
map_x = (int)(player.x / BLOCK);  // Örnek: 128 / 64 = 2
map_y = (int)(player.y / BLOCK);  // Örnek: 128 / 64 = 2

// Ray hangi yöne gidiyor?
dir_x = cos(angle);
dir_y = sin(angle);

// Her grid çizgisi arası mesafe
delta_x = fabs(1.0 / dir_x);
delta_y = fabs(1.0 / dir_y);
```

**2. İlk Grid Çizgisine Mesafe**

```c
// Sağa mı sola mı gidiyoruz?
if (dir_x < 0)  // Sola (Batıya)
{
    step_x = -1;  // Bir sonraki hücre: x-1
    // Oyuncudan soldaki grid çizgisine mesafe
    side_x = (player.x / BLOCK - map_x) * delta_x;
}
else  // Sağa (Doğuya)
{
    step_x = 1;  // Bir sonraki hücre: x+1
    // Oyuncudan sağdaki grid çizgisine mesafe
    side_x = (map_x + 1.0 - player.x / BLOCK) * delta_x;
}

// Aynı mantık Y için de geçerli
if (dir_y < 0)  // Yukarı (Kuzeye)
{
    step_y = -1;
    side_y = (player.y / BLOCK - map_y) * delta_y;
}
else  // Aşağı (Güneye)
{
    step_y = 1;
    side_y = (map_y + 1.0 - player.y / BLOCK) * delta_y;
}
```

**Görselleştirme:**
```
Grid hücre sınırları (BLOCK=64):

    |128    |192
    |       |
    +-------+-------+
    |       |       |
    |   P→→→|       |  Player: (150, 128)
    |       ↘       |  Angle: 45°
    +-------+-------+
           128
           
Hesaplama:
map_x = 150/64 = 2 (integer)
player.x / BLOCK = 150/64 = 2.34375

Sağdaki grid çizgisi: map_x + 1 = 3 → 192 pixel
Mesafe: (3.0 - 2.34375) * delta_x = 0.65625 * delta_x
```

**3. Ana Döngü (Grid Çizgilerinden Atlama)**

```c
while (!is_wall(map_x, map_y, game))
{
    // Hangi grid çizgisi daha yakın?
    if (side_x < side_y)
    {
        side_x += delta_x;  // Sonraki dikey çizgiye git
        map_x += step_x;    // X hücresini değiştir
        side = 0;           // Dikey duvara çarptık (E/W)
    }
    else
    {
        side_y += delta_y;  // Sonraki yatay çizgiye git
        map_y += step_y;    // Y hücresini değiştir
        side = 1;           // Yatay duvara çarptık (N/S)
    }
}
```

**Örnek İlerleme (45° açı):**
```
Başlangıç: map(2,2), side_x=0.93, side_y=0.93

Adım 1: side_x < side_y? Eşit! side_x seçelim
        → side_x = 0.93 + 1.41 = 2.34
        → map_x = 2 + 1 = 3
        → side = 0 (Dikey duvar)
        Şimdi: map(3,2), side_x=2.34, side_y=0.93

Adım 2: side_x < side_y? Hayır (2.34 > 0.93)
        → side_y = 0.93 + 1.41 = 2.34
        → map_y = 2 + 1 = 3
        → side = 1 (Yatay duvar)
        Şimdi: map(3,3), side_x=2.34, side_y=2.34

Adım 3: is_wall(3,3)? EVET! → Döngü bitti
        Son durum: side = 1 (NORTH veya SOUTH)
```

**4. Mesafe Hesaplama**

```c
if (side == 0)  // Dikey duvara çarptık (X grid çizgisi)
{
    wall_dist = (map_x - player.x / BLOCK + (1 - step_x) / 2) / cos(angle);
}
else  // Yatay duvara çarptık (Y grid çizgisi)
{
    wall_dist = (map_y - player.y / BLOCK + (1 - step_y) / 2) / sin(angle);
}
```

**Formül Açıklaması:**

```
map_x - player.x / BLOCK: Kaç grid hücresi geçtik?
(1 - step_x) / 2: Hücrenin hangi kenarındayız? (0 veya 0.5)
Toplam / cos(angle): Açıya göre gerçek mesafe
```

**5. Wall X Hesaplama (Texture Koordinatı İçin)**

```c
if (side == 0)  // Dikey duvar → Y koordinatı kullan
{
    wall_x = player.y / BLOCK + wall_dist * sin(angle) / BLOCK;
}
else  // Yatay duvar → X koordinatı kullan
{
    wall_x = player.x / BLOCK + wall_dist * cos(angle) / BLOCK;
}

wall_x -= floor(wall_x);  // 0.0 - 1.0 arasına normalize et
```

**Neden Bu Doğru?**

```
Örnek: Dikey duvara çarptık (side=0)
       Duvar X pozisyonu sabit (örn: 192)
       Texture Y ekseni boyunca değişiyor
       
Ray şu kadar ilerleyecek: wall_dist
Y yönünde ilerleme: wall_dist * sin(angle)
Başlangıç: player.y
Son nokta: player.y + wall_dist * sin(angle)

Bloğun içindeki pozisyon:
wall_x = (player.y + wall_dist * sin(angle)) / BLOCK
wall_x = wall_x - floor(wall_x)  → 0.0-1.0 aralığında
```

---

## 5. Kod Karşılaştırması

### A. Ray Initialization (Başlangıç)

**ESKİ (Pixel-Step):**
```c
ray_x = g->player.x;      // Dünya koordinatı (örn: 150.0)
ray_y = g->player.y;
step_x = cos(angle);      // Küçük adım (örn: 0.707)
step_y = sin(angle);
```

**YENİ (DDA):**
```c
// Grid koordinatları
ray->map_x = (int)(g->player.x / BLOCK);  // Grid hücresi (örn: 2)
ray->map_y = (int)(g->player.y / BLOCK);

// Delta değerleri (grid çizgileri arası)
ray->delta_x = fabs(1.0 / cos(angle));    // Büyük adım (örn: 1.414)
ray->delta_y = fabs(1.0 / sin(angle));

// Yön belirleme
if (cos(angle) < 0)
    ray->step_x = -1;  // Batı
else
    ray->step_x = 1;   // Doğu
```

**FARK:**
- Eski: Dünya koordinatlarında pixel-pixel
- Yeni: Grid koordinatlarında hücre-hücre

---

### B. Ana Döngü

**ESKİ (Pixel-Step):**
```c
while (!is_wall_hit(ray_x, ray_y, g))
{
    prev_x = ray_x;       // Her adımda kaydet
    prev_y = ray_y;
    ray_x += step_x;      // +0.707 gibi küçük adım
    ray_y += step_y;
}
// Döngü bitti → ray_x ve ray_y duvarın İÇİNDE!
// prev_x, prev_y kullan (yaklaşık)
```

**YENİ (DDA):**
```c
while (!is_wall(ray->map_x, ray->map_y, g))
{
    if (ray->side_x < ray->side_y)
    {
        ray->side_x += ray->delta_x;  // +1.414 gibi büyük adım
        ray->map_x += ray->step_x;     // Grid hücresi değişir
        ray->side = 0;                 // Dikey duvar
    }
    else
    {
        ray->side_y += ray->delta_y;
        ray->map_y += ray->step_y;
        ray->side = 1;                 // Yatay duvar
    }
}
// Döngü bitti → map_x, map_y tam olarak duvar hücresi
// side değişkeni hangi yüze çarptığımızı söylüyor
```

**FARK:**
- Eski: Her adımda is_wall_hit() çağrısı (yavaş)
- Yeni: Sadece grid hücresi değişince kontrol (hızlı)
- Eski: Yaklaşık konum (prev)
- Yeni: Kesin grid pozisyonu + yön bilgisi (side)

---

### C. Wall Face Belirleme

**ESKİ (Pixel-Step):**
```c
static int	get_wall_face(float hit_x, float hit_y, float step_x, float step_y)
{
    float dx = fmod(hit_x, BLOCK);  // Bloğun içinde neredeyiz?
    float dy = fmod(hit_y, BLOCK);
    
    // Kenarlara yakınlık kontrolü
    if (dx < 1.0)
        return (1);  // WEST
    if (dx > BLOCK - 1.0)
        return (0);  // EAST
    if (dy < 1.0)
        return (3);  // NORTH
    if (dy > BLOCK - 1.0)
        return (2);  // SOUTH
    
    // ⚠️ ORTADAYSA TAHMİN ET!
    if (fabs(step_x) > fabs(step_y))
        return (step_x > 0 ? 0 : 1);
    return (step_y > 0 ? 2 : 3);
}
```

**YENİ (DDA):**
```c
static t_texture	*select_texture(t_ray *ray, t_game *g)
{
    if (ray->side == 0)  // Dikey grid çizgisi
    {
        if (ray->step_x > 0)
            return (&g->e_tex);  // Doğuya gidiyoruz → EAST duvara çarptık
        return (&g->w_tex);      // Batıya gidiyoruz → WEST duvara çarptık
    }
    // Yatay grid çizgisi
    if (ray->step_y > 0)
        return (&g->s_tex);      // Güneye gidiyoruz → SOUTH duvara çarptık
    return (&g->n_tex);          // Kuzeye gidiyoruz → NORTH duvara çarptık
}
```

**FARK:**
- Eski: Tahmin + eşik kontrolü (hatalı olabilir)
- Yeni: Matematiksel kesinlik (side değişkeni)

---

### D. Texture X Koordinatı

**ESKİ (Pixel-Step):**
```c
static int	calc_tex_x(float hit_x, float hit_y, t_texture *tex, int face)
{
    float wall_hit;
    
    if (face == 0 || face == 1)  // E/W
        wall_hit = fmod(hit_y, BLOCK);  // hit_y ≈ 189.5 (yaklaşık!)
    else
        wall_hit = fmod(hit_x, BLOCK);
    
    if (wall_hit < 0)
        wall_hit += BLOCK;
    
    wall_hit = wall_hit / BLOCK;  // 0-1 normalize
    tex_x = (int)(wall_hit * tex->width);
    
    // Sınır kontrolleri
    if (tex_x < 0) tex_x = 0;
    if (tex_x >= tex->width) tex_x = tex->width - 1;
    
    return (tex_x);
}
```

**YENİ (DDA):**
```c
// 1. Wall X hesaplama (render_frame içinde)
if (ray->side == 0)  // Dikey duvar
    wall_x = player.y / BLOCK + wall_dist * sin(angle) / BLOCK;
else  // Yatay duvar
    wall_x = player.x / BLOCK + wall_dist * cos(angle) / BLOCK;

wall_x -= floor(wall_x);  // 0.0 - 1.0 (Matematiksel kesin!)

// 2. Texture X (draw_wall_column içinde)
tex_x = (int)(wall_x * tex->width);
if (tex_x < 0) tex_x = 0;
if (tex_x >= tex->width) tex_x = tex->width - 1;
```

**FARK:**
- Eski: `fmod(yaklaşık_değer, BLOCK)` → Tutarsız
- Yeni: Matematiksel formül → Her zaman doğru

---

### E. Performans

**ESKİ (Pixel-Step) - Örnek:**
```
Duvar 64 pixel uzakta
Her adım: 0.7 pixel
Toplam adım: 64 / 0.7 ≈ 91 adım
Her adımda is_wall_hit() çağrısı: 91 kez
```

**YENİ (DDA) - Aynı Örnek:**
```
Duvar 1 grid hücresi uzakta
Toplam adım: 1 adım
is_wall() çağrısı: 1 kez
```

**Hız Farkı:** DDA yaklaşık **90 kat** daha hızlı!

---

## 6. Wall Face (Duvar Yüzü) Tespiti

### Grid Sistemi ve Duvar Yüzleri

```
        NORTH (↑)
            |
    +-------+-------+
    |       |       |
WEST|   ?   |   1   |EAST
 (←)|       |       |(→)
    +-------+-------+
    |       |       |
    |   ?   |   ?   |
    +-------+-------+
            |
        SOUTH (↓)
```

**Anahtar Kavram:** Her grid hücresinin 4 yüzü var:
- **NORTH:** Üst kenar (Y azalırken çarpılan)
- **SOUTH:** Alt kenar (Y artarken çarpılan)
- **EAST:** Sağ kenar (X artarken çarpılan)
- **WEST:** Sol kenar (X azalırken çarpılan)

### Pixel-Step Sorunu

```c
// Eski kodunuz
if (dx < 1.0)           // Sol kenara çok yakın
    return (1);         // WEST
if (dx > BLOCK - 1.0)   // Sağ kenara çok yakın
    return (0);         // EAST
```

**Problem:**
```
Örnek: hit_x = 191.5, hit_y = 150.0
       Block sınırı: 192

dx = fmod(191.5, 64) = 63.5
dy = fmod(150.0, 64) = 22.0

dx > 63.0? EVET → EAST
AMA! Gerçekte ray 192'ye (dikey grid çizgisi) çarpmış olabilir
     veya 150'nin yatay çizgisine
     
Kod sadece "kenara yakın mı?" soruyor
"Hangi grid çizgisine çarptı?" sorusunu cevaplamıyor
```

### DDA Çözümü

DDA algoritması döngü içinde **tam olarak hangi grid çizgisine çarpıldığını** kaydediyor:

```c
if (ray->side_x < ray->side_y)
{
    // Dikey grid çizgisine ulaştık (X sabit)
    ray->side = 0;  // ✅ BU BİLGİ KAYIT EDİLİYOR!
    ray->map_x += ray->step_x;
}
else
{
    // Yatay grid çizgisine ulaştık (Y sabit)
    ray->side = 1;  // ✅ BU BİLGİ KAYIT EDİLİYOR!
    ray->map_y += ray->step_y;
}
```

**Texture Seçimi:**
```c
if (ray->side == 0)  // Dikey grid çizgisi (X çizgisi)
{
    if (ray->step_x > 0)
        return EAST_TEXTURE;  // Sağa giderken sol kenara çarptık
    else
        return WEST_TEXTURE;  // Sola giderken sağ kenara çarptık
}
else  // Yatay grid çizgisi (Y çizgisi)
{
    if (ray->step_y > 0)
        return SOUTH_TEXTURE;  // Aşağı giderken üst kenara çarptık
    else
        return NORTH_TEXTURE;  // Yukarı giderken alt kenara çarptık
}
```

**Neden Doğru?**

```
Senaryo: Ray doğuya (→) gidiyor (step_x = +1)

    Player
      ↓
    +-------+-------+-------+
    |       |       |DUVAR  |
    |   P→→→→→→→→→→→|1      |
    |       |       |       |
    +-------+-------+-------+
            ↑
       Dikey grid
       çizgisi
       (side=0)

Ray dikey çizgiye çarptı → side = 0
Ray sağa gidiyordu → step_x = +1
Demek ki duvarın BATI (WEST) yüzüne çarpmış!
Ama texture olarak EAST seç (dışarıdan bakış)
```

---

## 7. Texture Koordinat Hesabı

### Texture Mapping Teorisi

Bir texture'ı duvara yapıştırmak için 2 koordinata ihtiyacımız var:
- **tex_x:** Texture'ın yatay konumu (0 - texture.width)
- **tex_y:** Texture'ın dikey konumu (0 - texture.height)

```
Texture (64x64):        Duvar Render:
┌──────────┐            ┌──────────┐
│ tex_y=0  │            │          │ ← Tavan
│    ↓     │            │          │
│ ░░▒▒▓▓██ │   ──────>  │ ░░▒▒▓▓██ │
│ ░░▒▒▓▓██ │            │ ░░▒▒▓▓██ │
│ tex_y=63 │            │          │ ← Zemin
└──────────┘            └──────────┘
 tex_x=0...63            Ekran X
```

### Wall X Hesaplama (Dikey Pozisyon)

**Amaç:** Duvar yüzeyinde tam olarak nerede olduğumuzu bul (0.0 - 1.0)

**Matematiksel Formül:**

```
Dikey duvar için (side = 0):
    wall_x = (player.y + ray_distance * sin(angle)) / BLOCK
    wall_x = wall_x - floor(wall_x)

Yatay duvar için (side = 1):
    wall_x = (player.x + ray_distance * cos(angle)) / BLOCK
    wall_x = wall_x - floor(wall_x)
```

**Adım Adım Örnek:**

```
Senaryo:
- Player: (150, 128)
- Angle: 45°
- Wall distance: 90 pixel
- Side: 0 (dikey duvar)

Hesaplama:
1. Ray'in Y yönünde ilerlemesi:
   y_offset = 90 * sin(45°) = 90 * 0.707 = 63.63

2. Ray'in bittiği Y koordinatı:
   final_y = 128 + 63.63 = 191.63 pixel

3. Grid koordinatına çevir:
   wall_x = 191.63 / 64 = 2.9942

4. Tam sayı kısmını çıkar (grid numarasını at):
   wall_x = 2.9942 - 2 = 0.9942

5. Texture koordinatına çevir:
   tex_x = 0.9942 * 64 = 63.6 ≈ 63
```

**Görselleştirme:**
```
Grid Hücre 2 (Y: 128-192):
┌─────────────────────────────────┐
│ 0.0                         1.0 │ ← wall_x aralığı
│  ↓                           ↓  │
│                              •  │ ← wall_x = 0.9942
│                                 │
│ Player ────────────────────→ •  │
│ (128)                     (191.63)
└─────────────────────────────────┘
  128                           192

Texture'da:
tex[63] ← 0.9942 * 64 = 63 nolu sütun
```

### Pixel-Step ile Wall X Problemi

```c
// Eski kodunuz
wall_hit = fmod(hit_y, BLOCK);  // hit_y = prev_y (yaklaşık!)
wall_hit = wall_hit / BLOCK;
tex_x = (int)(wall_hit * tex->width);
```

**Problem:**
```
Aynı duvar için 3 farklı ray:

Ray 1: prev_y = 189.3
       wall_hit = fmod(189.3, 64) = 61.3
       tex_x = 61.3/64 * 64 = 61

Ray 2: prev_y = 190.1
       wall_hit = fmod(190.1, 64) = 62.1
       tex_x = 62.1/64 * 64 = 62

Ray 3: prev_y = 191.8
       wall_hit = fmod(191.8, 64) = 63.8
       tex_x = 63.8/64 * 64 = 63

Ekranda:
┌─┬─┬─┐
│61│62│63│ ← Texture sütunları
└─┴─┴─┘
Ama bunlar aynı duvar noktası! 
Texture'ın farklı yerlerinden örnekleme yapıldı → Şeritlenme!
```

### DDA ile Wall X Çözümü

```c
// Yeni kod
if (ray->side == 0)
    wall_x = g->player.y / BLOCK + ray->wall_dist * sin(ray->angle) / BLOCK;
else
    wall_x = g->player.x / BLOCK + ray->wall_dist * cos(ray->angle) / BLOCK;

wall_x -= floor(wall_x);
tex_x = (int)(wall_x * tex->width);
```

**Neden Doğru?**
```
Aynı duvar için 3 ray (DDA):

Ray 1: angle=44.5°, wall_dist=90.1
       wall_x = 128/64 + 90.1 * sin(44.5°) / 64
       wall_x = 2.0 + 0.9935 = 2.9935
       wall_x = 0.9935
       tex_x = 0.9935 * 64 = 63

Ray 2: angle=45.0°, wall_dist=90.5
       wall_x = 128/64 + 90.5 * sin(45.0°) / 64
       wall_x = 2.0 + 0.9987 = 2.9987
       wall_x = 0.9987
       tex_x = 0.9987 * 64 = 63

Ray 3: angle=45.5°, wall_dist=91.0
       wall_x = 128/64 + 91.0 * sin(45.5°) / 64
       wall_x = 2.0 + 1.0124 = 3.0124
       wall_x = 0.0124  ← Yeni bloğa geçti!
       tex_x = 0.0124 * 64 = 0

Ekranda:
┌─┬─┬─┐
│63│63│0│ ← Smooth geçiş! (bloğun sonundan başlangıcına)
└─┴─┴─┘
```

### Texture Y Hesaplama

Texture Y koordinatı duvar yüksekliği boyunca değişir:

```c
int wall_height = (int)(BLOCK / wall_dist * (WIDTH / 2));
float step = (float)tex->height / wall_height;
float tex_pos = 0.0;

for (int y = start; y < end; y++)
{
    int tex_y = (int)tex_pos;
    // Texture pixel'ini al: tex->data[tex_y * tex->width + tex_x]
    tex_pos += step;
}
```

**Mantık:**
```
Duvar ekranda 200 pixel yüksekliğinde
Texture 64 pixel yüksekliğinde

Step = 64 / 200 = 0.32

Ekran Y=0  → tex_pos=0.00  → tex_y=0
Ekran Y=1  → tex_pos=0.32  → tex_y=0
Ekran Y=2  → tex_pos=0.64  → tex_y=0
Ekran Y=3  → tex_pos=0.96  → tex_y=0
Ekran Y=4  → tex_pos=1.28  → tex_y=1  ← Texture pixel değişti
...
```

---

## 8. Performans Analizi

### Benchmark Karşılaştırması

**Test Senaryosu:** 1280x720 çözünürlük, ortalama duvar mesafesi 300 pixel

**Pixel-Step (Eski Kod):**
```
Hesaplama (her ray için):
- Adım sayısı: 300 / 0.7 ≈ 428 adım
- is_wall_hit() çağrısı: 428 kez
- cos/sin hesabı: 2 kez (başlangıçta)
- fmod() hesabı: 3 kez (wall_face + tex_x)

Toplam (1280 ray):
- is_wall_hit(): 428 * 1280 = 547,840 çağrı
- fmod(): 3 * 1280 = 3,840 çağrı
```

**DDA (Yeni Kod):**
```
Hesaplama (her ray için):
- Adım sayısı: 300 / 64 ≈ 5 adım
- is_wall(): 5 kez
- cos/sin hesabı: 2 kez
- floor() hesabı: 1 kez
- fabs() hesabı: 2 kez (başlangıçta)

Toplam (1280 ray):
- is_wall(): 5 * 1280 = 6,400 çağrı
- floor(): 1 * 1280 = 1,280 çağrı
```

**Karşılaştırma:**
```
İşlem             | Pixel-Step | DDA     | Fark
------------------|------------|---------|--------
Duvar kontrolü    | 547,840    | 6,400   | 85x hızlı
Toplam işlem      | ~600,000   | ~10,000 | 60x hızlı
```

### Bellek Kullanımı

**Pixel-Step:**
```c
float ray_info[5];  // 5 * 4 byte = 20 byte
float prev_x, prev_y;  // 8 byte
Toplam: 28 byte
```

**DDA:**
```c
typedef struct s_ray
{
    float angle;      // 4 byte
    int   map_x;      // 4 byte
    int   map_y;      // 4 byte
    float side_x;     // 4 byte
    float side_y;     // 4 byte
    float delta_x;    // 4 byte
    float delta_y;    // 4 byte
    int   step_x;     // 4 byte
    int   step_y;     // 4 byte
    int   side;       // 4 byte
    float wall_dist;  // 4 byte
    float wall_x;     // 4 byte
}   t_ray;
Toplam: 48 byte
```

**Sonuç:** DDA 20 byte daha fazla bellek kullanıyor ama %600 daha hızlı!

### FPS (Frame Per Second) Etkisi

**Tahmin:**
```
Pixel-Step FPS: ~30-40 FPS
DDA FPS: ~200-300 FPS

Artış: ~7-8x
```

---

## 9. Özet ve Sonuç

### Ana Problemler (Pixel-Step)

1. **Aşırı Örnekleme**
   - Aynı grid hücresinde 50-100 adım
   - Her adımda duvar kontrolü
   - Performans kaybı

2. **Yaklaşık Hit Noktası**
   - `prev_x`, `prev_y` kullanımı
   - Gerçek kesişim noktası bilinmiyor
   - Texture koordinatları tutarsız

3. **Wall Face Belirsizliği**
   - `fmod()` + eşik kontrolü
   - Tahmine dayalı seçim
   - Yanlış texture seçimi

4. **Texture Bozulması**
   - Aynı duvar için farklı tex_x değerleri
   - Dikey şeritlenme
   - Tekrar eden desenler

### Çözümler (DDA)

1. **Grid-Based İlerleme**
   ```
   Pixel-pixel değil → Grid çizgilerinden çizgiye
   ```

2. **Matematiksel Kesinlik**
   ```
   Yaklaşık değil → Formülle hesaplanan kesin nokta
   ```

3. **Side Değişkeni**
   ```
   Tahmin değil → Hangi grid çizgisine çarpıldığı kesin
   ```

4. **Normalize Wall X**
   ```
   fmod(yaklaşık) değil → Matematiksel normalize (0.0-1.0)
   ```

### Öğrenilen Dersler

**1. Algoritma Seçimi Kritik**
```
Problem: "Kod çalışıyor ama yavaş ve hatalı"
Çözüm: Başka bir algoritma kullan
Sonuç: 60x hızlı + hatasız
```

**2. Matematiksel Doğruluk**
```
Yaklaşık değerler: prev_x, prev_y → Texture bozulması
Kesin değerler: Formüle dayalı → Mükemmel görüntü
```

**3. Veri Yapısı Tasarımı**
```
Pixel-Step: ray_info[5] → Belirsiz, geçici
DDA: t_ray struct → Net, anlamlı
```

### Sonraki Adımlar

**Kodunuzu DDA'ya geçirmek için:**

1. ✅ `raycast.c` dosyasını yeniden yaz
2. ✅ `t_ray` struct'ını header'a ekle
3. ✅ Eski fonksiyonları kaldır
4. ✅ Test et

**Beklenen Sonuç:**
- ✅ Texture'lar düzgün render edilecek
- ✅ Dikey şeritlenme kaybolacak
- ✅ FPS artacak
- ✅ Kod daha okunabilir olacak

---

## Ek Kaynaklar

### Görsel Karşılaştırma

```
Pixel-Step Raycasting:
┌────────────────────────────────┐
│ ██ ██ ██ ██ ██ ██ ██ ██ ██    │
│ ██ ██ ██ ██ ██ ██ ██ ██ ██    │  ← Şeritli, bozuk
│ ██ ██ ██ ██ ██ ██ ██ ██ ██    │
└────────────────────────────────┘

DDA Raycasting:
┌────────────────────────────────┐
│ ████████████████████████████    │
│ ████████████████████████████    │  ← Düzgün, kesintisiz
│ ████████████████████████████    │
└────────────────────────────────┘
```

### Formül Özeti

**DDA Delta:**
```
delta_x = |1 / cos(angle)|
delta_y = |1 / sin(angle)|
```

**Başlangıç Mesafesi:**
```
side_x = (next_grid_x - current_pos_x) * delta_x
side_y = (next_grid_y - current_pos_y) * delta_y
```

**Duvar Mesafesi:**
```
if (side == 0):
    dist = (map_x - player_x + offset) / cos(angle)
else:
    dist = (map_y - player_y + offset) / sin(angle)
```

**Wall X:**
```
if (side == 0):
    wall_x = player_y + dist * sin(angle)
else:
    wall_x = player_x + dist * cos(angle)

wall_x = (wall_x / BLOCK) - floor(wall_x / BLOCK)
```

**Texture Koordinatı:**
```
tex_x = (int)(wall_x * texture_width)
tex_y = (int)(tex_pos) where tex_pos += step
```

---

## Son Notlar

Bu dokümanda açıklanan DDA algoritması:
- ✅ 42 Norm uyumlu
- ✅ Matematiksel olarak doğru
- ✅ Performans açısından optimal
- ✅ Bakımı kolay, anlaşılır

Kodunuzu DDA'ya geçirdiğinizde texture sorunlarınız tamamen çözülecektir.

**Başarılar!** 🎮🚀
