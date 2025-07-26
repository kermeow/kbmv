project('kbmv', 'cpp', version: '0.1', default_options: [
    'default_library=static'
])

src = ['src/main.cpp']
deps = [dependency('raylib'), dependency('sfml-window'), dependency('tomlplusplus')]

executable('kbmv', src, dependencies: deps)