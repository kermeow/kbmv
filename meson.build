project('kbmv', 'cpp', version: '0.1', default_options: ['default_library=static'])

deps = [
    dependency('raylib'),
    dependency('sfml-system'),
    dependency('sfml-window'),
    dependency('tomlplusplus'),
]

subdir('./src')
executable('kbmv', src, dependencies: deps)
