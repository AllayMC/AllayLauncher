add_rules('mode.debug', 'mode.release')

if is_plat('linux') then
    set_toolchains('zig')
    add_requireconfs('**', { system = false })

    add_requireconfs('cpr', {configs = {ssl = true}})
    add_requireconfs('cpr.libcurl', {configs = {openssl = true}})
end

add_requires('argparse      3.1')
add_requires('spdlog        1.14.1')
add_requires('nlohmann_json 3.11.3')
add_requires('cpr           1.11.1')

target('allay')
    set_kind('binary')
    add_files('src/**.cpp')
    add_includedirs('src')
    add_includedirs('build/config')
    set_version("1.0.0")

    add_packages(
        'argparse',
        'spdlog',
        'nlohmann_json',
        'cpr'
    )

    if is_plat('windows') then
        add_links('winhttp.lib')
        add_cxflags('/utf-8')

        remove_files('src/**_linux.*')
    else
        remove_files('src/**_win32.*')
    end

    set_languages('c++17')
    set_optimize('fastest')

    set_pcxxheader('src/pch.h')

    set_configdir('$(buildir)/config')
    add_configfiles('src/config.h.in')

    if is_mode('debug') then 
        add_defines('DEBUG')
    end
