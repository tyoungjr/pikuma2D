project('pikuma2D', 'cpp',
  version : '0.1',
  default_options : [ 'warning_level=3', 
                      'cpp_std=c++17',
                      'default_library=static' ])

# meson setup --wipe build if any errors persist
sdl2_dep = dependency('sdl2')
glm_dep = dependency('glm')
imgui_dep = dependency('imgui')
sdl2_img_dep = dependency('sdl2_image')
sdl2_mix_dep = dependency('sdl2_mixer')
sdl2_ttf_dep = dependency('sdl2_ttf')
lua_dep = dependency('lua', fallback: [ 'lua', 'lua_dep' ])

# Expose standard dependency.
sol2_dep = declare_dependency(
  include_directories: include_directories('./include'),
  dependencies: [ lua_dep ],
)


incdir = include_directories('include')
src = ['src/Logger.cpp', 'src/Game.cpp', 'src/Main.cpp', 'src/ECS.cpp']

deps = [sdl2_dep, glm_dep, sdl2_img_dep, imgui_dep, sol2_dep, sdl2_mix_dep,
        sdl2_ttf_dep]
executable('pikuma2D',
           sources: src,
           include_directories: incdir,
           dependencies:deps,
           install : true)
