project('kbmv', 'cpp', version: '0.1', default_options: [
    'default_library=static'
])

src = ['src/main.cpp', 'src/config.cpp', 'src/window.cpp', 'src/input.cpp', 'src/layout.cpp']
deps = [dependency('raylib'), dependency('sfml-system'), dependency('sfml-window'), dependency('tomlplusplus')]

executable('kbmv', src, dependencies: deps)