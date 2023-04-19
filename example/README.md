# SDLit Examples

Collection of small application to demonstrate SDLit usage.

To run them, enable building them using build\_example option:
```bash
meson setup builddir --reconfigure --buildtype=release -Dexample=enabled

meson compile -C builddir
```

## Play Music

```bash
./builddir/play_music <Music>
```

## Play Chunk

```bash
./builddir/play_chunk <Music>
```

## Image Viewer

```bash
./builddir/image_viewer <Image>
```

![Hokusai's Under the Wave off Kanagawa open on image viewer](image_viewer.jpg "Hokusai's Under the Wave off Kanagawa")

## Show Text

```bash
./builddir/show_text <Message> <Font File>.ttf <Font Size>
```

![Hello, World! displayed on show text](show_text.jpg "Hello, World!")

## SMPTE Test

```bash
./builddir/smpte_test
```

![TV SMPTE color bar test](smpte.jpg "TV SMPTE")
