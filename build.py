import os
from conan.packager import ConanMultiPackager


if __name__ == "__main__":
    # ConanPackageTools
    # See: https://github.com/conan-io/conan-package-tools/blob/develop/README.md
    builder = ConanMultiPackager()
    builder.add_common_builds()
    updated_builds = []
    # Add environment variables to build definitions
    xms_version = os.environ.get('XMS_VERSION', None)
    python_target_version = os.environ.get('PYTHON_TARGET_VERSION', "3.6")

    for settings, options, env_vars, build_requires, reference in builder.items:
        env_vars.update({
            'XMS_VERSION': xms_version,
            'VERBOSE': 1,
            'PYTHON_TARGET_VERSION': python_target_version
        })

        # Require c++11 compatibility
        if settings['compiler'] == 'gcc':
            settings.update({
                'compiler.libcxx': 'libstdc++11'
            })

    pybind_updated_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:
        # pybind option
        if (not settings['compiler'] == "Visual Studio" \
                     or int(settings['compiler.version']) > 12) \
                and settings['arch'] == "x86_64" and settings['build_type'] != 'Debug':
            pybind_options = dict(options)
            pybind_options.update({'xmsmesh:pybind': True})
            pybind_updated_builds.append([settings, pybind_options, env_vars, build_requires])

        pybind_updated_builds.append([settings, options, env_vars, build_requires])
    builder.builds = pybind_updated_builds

    xms_updated_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:
        # xms option
        if settings['compiler'] == 'Visual Studio' \
                and 'MD' in settings['compiler.runtime'] \
                and int(settings['compiler.version']) < 13:
            xms_options = dict(options)
            xms_options.update({'xmsmesh:xms': True})
            xms_updated_builds.append([settings, xms_options, env_vars, build_requires])
        xms_updated_builds.append([settings, options, env_vars, build_requires])
    builder.builds = xms_updated_builds

    testing_updated_builds = []
    for settings, options, env_vars, build_requires, reference in builder.items:
        # testing option
        if not options.get('xmsmesh:xms', False) and not options.get('xmsmesh:pybind', False):
            testing_options = dict(options)
            testing_options.update({'xmsmesh:testing': True})
            testing_updated_builds.append([settings, testing_options, env_vars, build_requires])
        testing_updated_builds.append([settings, options, env_vars, build_requires])
    builder.builds = testing_updated_builds

    builder.run()
