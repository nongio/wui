project('yoga', 'cpp',
  meson_version: '>=0.58.1',
  default_options: [
    'cpp_std=c++17',
  ],
)
headers = [
	'./yoga/CompactValue.h',
	'./yoga/log.h',
	'./yoga/Utils.h',
	'./yoga/YGValue.h',
	'./yoga/YGNode.h',
	'./yoga/YGEnums.h',
	'./yoga/BitUtils.h',
	'./yoga/internal/experiments.h',
	'./yoga/internal/experiments-inl.h',
	'./yoga/YGFloatOptional.h',
	'./yoga/Yoga-internal.h',
	'./yoga/YGLayout.h',
	'./yoga/YGStyle.h',
	'./yoga/YGConfig.h',
	'./yoga/YGNodePrint.h',
	'./yoga/event/event.h',
	'./yoga/Yoga.h',
	'./yoga/YGMacros.h',
]
sources = [
	'./yoga/Utils.cpp',
	'./yoga/log.cpp',
	'./yoga/YGValue.cpp',
	'./yoga/YGLayout.cpp',
	'./yoga/Yoga.cpp',
	'./yoga/YGStyle.cpp',
	'./yoga/internal/experiments.cpp',
	'./yoga/YGNode.cpp',
	'./yoga/YGNodePrint.cpp',
	'./yoga/event/event.cpp',
	'./yoga/YGEnums.cpp',
	'./yoga/YGConfig.cpp',
]
lib_yoga = static_library(
	'yoga',
	sources + headers,
)

yoga = declare_dependency(
	link_with: lib_yoga,
	sources: headers,
)
