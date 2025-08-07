add_rules('mode.debug', 'mode.release')

if is_plat('linux') then
    set_toolchains('zig')
    add_requireconfs('**', { system = false })

    add_requireconfs('cpr', {configs = {ssl = true}})
    add_requireconfs('cpr.libcurl', {configs = {openssl = true}})
end

add_requires('argparse      3.2')
add_requires('spdlog        1.15.3')
add_requires('nlohmann_json 3.12.0')
add_requires('cpr           1.11.2')
add_requires('openssl       1.1.1-w')
add_requires('gtest         1.17.0')

add_files('src/**.cpp')
add_includedirs('src')

--- common settings

add_packages(
    'argparse',
    'spdlog',
    'nlohmann_json',
    'cpr',
    'openssl'
)

set_version('1.1.0')

if is_plat('windows', 'mingw') then
    add_links('winhttp')
    add_cxflags('/utf-8', {tools = {'clang_cl', 'cl'}})

    add_files('assets/app.rc')
    remove_files('src/**_linux.*')
else
    remove_files('src/**_win32.*')
end

set_languages('c++17')
set_optimize('smallest')

set_configdir('$(buildir)/config')
add_configfiles('src/config.h.in')

if is_mode('debug') then
    add_defines('AL_DEBUG')
end

--- targets

target('allay')
    set_kind('binary')
    add_includedirs('build/config')
    set_pcxxheader('src/pch.h')

target('googletest')
    set_kind('binary')
    set_default(false)
    set_pcxxheader('src/pch.h')

    add_files('tests/**.cpp')
    add_includedirs('tests')
    remove_files('src/main.cpp')

    add_packages('gtest')
    add_tests("default")
