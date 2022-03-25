project('sc-test', 'cpp', 'c',
  version: '0.0.0',
  license: 'MIT',
  meson_version: '>=0.58.1',
  default_options: [
    'c_std=c11',
    'cpp_std=c++17',
#    'warning_level=2',
#    'werror=true',
  ],
)

add_project_arguments(
  [
    '-DWLR_USE_UNSTABLE',
    '-Wundef',
    '-Wno-unused-parameter',
  ],
  language: 'cpp',
)

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language : 'c')
endif

cc = meson.get_compiler('c')

wayland_protos = dependency('wayland-protocols', version: '>=1.14')
wayland_egl = dependency('wayland-egl')
egl = dependency('egl')
glesv2 = dependency('glesv2')

# Depot tools
depot_tools = subproject('depot_tools')
# Skia
libskia = subproject('skia').get_variable('dep')

wl_protocol_dir = wayland_protos.get_variable(pkgconfig:'pkgdatadir')

wayland_scanner = find_program('wayland-scanner')

wayland_scanner_server = generator(
  wayland_scanner,
  output: '@BASENAME@-protocol.h',
  arguments: ['server-header', '@INPUT@', '@OUTPUT@'],
)

server_protocols = [
  [wl_protocol_dir, 'stable/xdg-shell/xdg-shell.xml'],
 # ['../screen-composer/protocols/wlr-layer-shell-unstable-v1.xml'],
 # ['../screen-composer/protocols/sc-layer-unstable-v1.xml'],
]

wl_protos_src = []
wl_protos_headers = []

foreach p : server_protocols
	xml = join_paths(p)
	wl_protos_src += custom_target(
		xml.underscorify() + '_client_c',
		input: xml,
		output: '@BASENAME@-protocol.c',
		command: [wayland_scanner, 'public-code', '@INPUT@', '@OUTPUT@'],
	)
	wl_protos_headers += custom_target(
		xml.underscorify() + '_client_h',
		input: xml,
		output: '@BASENAME@-protocol.h',
		command: [wayland_scanner, 'client-header', '@INPUT@', '@OUTPUT@'],
	)
endforeach

lib_client_protos = static_library(
	'client_protos',
	wl_protos_src + wl_protos_headers,
	dependencies: wayland_egl.partial_dependency(compile_args: true),
)

client_protos = declare_dependency(
	link_with: lib_client_protos,
	sources: wl_protos_headers,
)


version = '@0@'.format(meson.project_version())
git = find_program('git', native: true, required: false)
if git.found()
  git_commit = run_command([git, 'rev-parse', '--short', 'HEAD'])
  git_branch = run_command([git, 'rev-parse', '--abbrev-ref', 'HEAD'])
  if git_commit.returncode() == 0 and git_branch.returncode() == 0
    version = '@0@-@1@ (branch \'@2@\')'.format(
      meson.project_version(),
      git_commit.stdout().strip(),
      git_branch.stdout().strip(),
    )
  endif
endif

#inih_dep = subproject('inih', default_options : []).get_variable('inih_dep')

conf_data = configuration_data()
conf_data.set_quoted('SC_VERSION', version)

sc_sources = [
  'main.cpp',
  'src/window.cpp',
  'src/egl.cpp',
  'src/skia.cpp',
  'src/output.cpp',
  'src/seat.cpp',
  'src/keyboard.cpp',
  'src/pointer.cpp',
  'src/wl.cpp',
]

sc_headers = [
  configure_file(input: 'config.h.in',
                 output: 'config.h',
                 configuration: conf_data),
]

executable(
  'layers',
  sc_sources,
  include_directories: include_directories('include'),
  dependencies: [
    client_protos,
    wayland_egl,
    libskia,
    egl,
    glesv2,
  #  dependency('libwebp'),
    dependency('libjpeg'),
    dependency('libpng'),
    dependency('fontconfig'),
    dependency('xkbcommon'),
  ],
  install: true,
)

summary = [
  '',
  'Layers @0@'.format(version),
  '',
  ''
]
message('\n'.join(summary))
