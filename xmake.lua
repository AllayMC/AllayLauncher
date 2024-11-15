add_rules("mode.debug", "mode.release")

target("allay")
    set_kind("binary")
    add_files("src/*.cpp")
    set_languages("c++23")
    set_optimize("smallest")