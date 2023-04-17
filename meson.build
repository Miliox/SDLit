project('SDLit', 'cpp',
  default_options: ['default_library=static', 'cpp_std=c++20'],
  license:         'MIT',
  version:         '0.0.1')

# Dependency to sdl2
sdl2_dep       = dependency('sdl2')
sdl2_image_dep = dependency('sdl2_image')
sdl2_mixer_dep = dependency('sdl2_mixer')
sdl2_ttf_dep   = dependency('sdl2_ttf')

sdlit_inc = include_directories('include')
sdlit_src = ['src/SDLit.cpp']
sdlit_dep = declare_dependency(
  include_directories: sdlit_inc,
  dependencies: [sdl2_dep, sdl2_image_dep, sdl2_mixer_dep, sdl2_ttf_dep],
)
sdlit_lib = static_library('sdlit', sdlit_src, dependencies: sdlit_dep)

if get_option('build_example').enabled()
  example_dep = declare_dependency(dependencies: sdlit_dep, link_with: sdlit_lib)

  # rotate around the different system cursors
  executable('cursor_test', 'example/cursor_test.cpp', win_subsystem: 'windows', dependencies: example_dep)

  # list system available locales
  executable('list_locale', 'example/list_locale.cpp', win_subsystem: 'windows', dependencies: example_dep)

  # simple image viewer with reactive resizable window
  executable('image_viewer', 'example/image_viewer.cpp', win_subsystem: 'windows', dependencies: example_dep)

  # decode the whole input file before playing it
  executable('play_chunk', 'example/play_chunk.cpp', win_subsystem: 'windows', dependencies: example_dep)

  # decode the input file on demand while playing it
  executable('play_music', 'example/play_music.cpp', win_subsystem: 'windows', dependencies: example_dep)

  # create a window with colored stripes used to color test TVs
  executable('smpte_test', 'example/smpte_test.cpp', win_subsystem: 'windows', dependencies: example_dep)

  # display a given string in a window
  executable('show_text', 'example/show_text.cpp', win_subsystem: 'windows', dependencies: example_dep)
endif

