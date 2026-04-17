pipeline {
    agent any

    environment {
        SPDLOG_COMMIT           = '79524ddd08a4ec981b7fea76afd08ee05f83755d'  // tag 1.7.0
        TOML11_COMMIT           = 'be08ba2be2a964edcdb3d3e3ea8d100abc26f286'  // tag v4.4.0
        GOOGLETEST_COMMIT       = '52eb8108c5bdec04579160ae17225d66034bd723'  // tag v1.17.0
        CLI11_COMMIT            = '37bb6edc5317e99af72ef48405e65d9ca5218861'  // tag v2.6.2
        IXWEBSOCKET_COMMIT      = '2efe037c9cc96fd536774f17bdb5215161ee5087'  // tag v11.4.6
        NLOHMANN_JSON_COMMIT    = '55f93686c01528224f448c19128836e7df245f72'  // tag v3.12.0
        DEPS_PREFIX_PATH        = "${WORKSPACE}/deps/spdlog;" +
                                  "${WORKSPACE}/deps/toml11;" +
                                  "${WORKSPACE}/deps/googletest;" +
                                  "${WORKSPACE}/deps/cli11;" +
                                  "${WORKSPACE}/deps/ixwebsocket;" +
                                  "${WORKSPACE}/deps/nlohmann-json"
    }

    stages {
        stage('Build spdlog') {
            steps {
                dir('deps/spdlog/.src') {
                    checkout([
                        $class: 'GitSCM',
                        branches: [[name: "${SPDLOG_COMMIT}"]],
                        userRemoteConfigs: [[url: 'https://github.com/gabime/spdlog.git']]
                    ])
                }
                sh """
                    cmake -G Ninja -B "${WORKSPACE}/deps/spdlog/.build" -S "${WORKSPACE}/deps/spdlog/.src" \
                        -DCMAKE_INSTALL_PREFIX="${WORKSPACE}/deps/spdlog" \
                        -DSPDLOG_BUILD_EXAMPLE=OFF \
                        -DSPDLOG_BUILD_EXAMPLE_HO=OFF \
                        -DSPDLOG_BUILD_SHARED=OFF \
                        -DSPDLOG_BUILD_PIC=OFF \
                        -DSPDLOG_BUILD_TESTS=OFF \
                        -DSPDLOG_BUILD_TESTS_HO=OFF \
                        -DSPDLOG_BUILD_WARNINGS=OFF \
                        -DSPDLOG_DISABLE_DEFAULT_LOGGER=ON \
                        -DSPDLOG_INSTALL=ON \
                        -DSPDLOG_FMT_EXTERNAL_HO=OFF \
                        -DSPDLOG_USE_STD_FORMAT=ON \
                        -DSPDLOG_FMT_EXTERNAL=OFF
                """
                sh "cmake --build '${WORKSPACE}/deps/spdlog/.build'"
                sh "cmake --install '${WORKSPACE}/deps/spdlog/.build'"
            }
            post {
                always {
                    sh "rm -rf '${WORKSPACE}/deps/spdlog/.src' '${WORKSPACE}/deps/spdlog/.build'"
                }
            }
        }

        stage('Build toml11') {
            steps {
                dir('deps/toml11/.src') {
                    checkout([
                        $class: 'GitSCM',
                        branches: [[name: "${TOML11_COMMIT}"]],
                        userRemoteConfigs: [[url: 'https://github.com/ToruNiina/toml11.git']]
                    ])
                }
                sh """
                    cmake -G Ninja -B "${WORKSPACE}/deps/toml11/.build" -S "${WORKSPACE}/deps/toml11/.src" \
                        -DCMAKE_INSTALL_PREFIX="${WORKSPACE}/deps/toml11" \
                        -Dtoml11_BUILD_TESTS=OFF
                """
                sh "cmake --build '${WORKSPACE}/deps/toml11/.build'"
                sh "cmake --install '${WORKSPACE}/deps/toml11/.build'"
            }
            post {
                always {
                    sh "rm -rf '${WORKSPACE}/deps/toml11/.src' '${WORKSPACE}/deps/toml11/.build'"
                }
            }
        }

        stage('Build googletest') {
            steps {
                dir('deps/googletest/.src') {
                    checkout([
                        $class: 'GitSCM',
                        branches: [[name: "${GOOGLETEST_COMMIT}"]],
                        userRemoteConfigs: [[url: 'https://github.com/google/googletest.git']]
                    ])
                }
                sh """
                    cmake -G Ninja -B "${WORKSPACE}/deps/googletest/.build" -S "${WORKSPACE}/deps/googletest/.src" \
                        -DCMAKE_INSTALL_PREFIX="${WORKSPACE}/deps/googletest" \
                        -DINSTALL_GTEST=1 \
                        -Dgtest_force_shared_crt=1 \
                        -DCMAKE_CXX_STANDARD=20 \
                        -DBUILD_SHARED_LIBS=OFF \
                        -DBUILD_GMOCK=OFF
                """
                sh "cmake --build '${WORKSPACE}/deps/googletest/.build'"
                sh "cmake --install '${WORKSPACE}/deps/googletest/.build'"
            }
            post {
                always {
                    sh "rm -rf '${WORKSPACE}/deps/googletest/.src' '${WORKSPACE}/deps/googletest/.build'"
                }
            }
        }

        stage('Build CLI11') {
            steps {
                dir('deps/cli11/.src') {
                    checkout([
                        $class: 'GitSCM',
                        branches: [[name: "${CLI11_COMMIT}"]],
                        userRemoteConfigs: [[url: 'https://github.com/CLIUtils/CLI11.git']]
                    ])
                }
                sh """
                    cmake -G Ninja -B "${WORKSPACE}/deps/cli11/.build" -S "${WORKSPACE}/deps/cli11/.src" \\
                        -DCMAKE_INSTALL_PREFIX="${WORKSPACE}/deps/cli11" \\
                        -DCLI11_BUILD_DOCS=OFF \\
                        -DCLI11_BUILD_EXAMPLES=OFF \\
                        -DCLI11_BUILD_TESTS=OFF \\
                        -DCLI11_INSTALL=ON \\
                        -DCLI11_PRECOMPILED=OFF \\
                        -DCLI11_SANITIZERS=OFF \\
                        -DCLI11_SINGLE_FILE=OFF
                """
                sh "cmake --build '${WORKSPACE}/deps/cli11/.build'"
                sh "cmake --install '${WORKSPACE}/deps/cli11/.build'"
            }
            post {
                always {
                    sh "rm -rf '${WORKSPACE}/deps/cli11/.src' '${WORKSPACE}/deps/cli11/.build'"
                }
            }
        }

        stage('Build IXWebSocket') {
            steps {
                dir('deps/ixwebsocket/.src') {
                    checkout([
                        $class: 'GitSCM',
                        branches: [[name: "${IXWEBSOCKET_COMMIT}"]],
                        userRemoteConfigs: [[url: 'https://github.com/machinezone/IXWebSocket.git']]
                    ])
                }
                sh """
                    cmake -G Ninja -B "${WORKSPACE}/deps/ixwebsocket/.build" -S "${WORKSPACE}/deps/ixwebsocket/.src" \\
                        -DCMAKE_INSTALL_PREFIX="${WORKSPACE}/deps/ixwebsocket" \\
                        -DUSE_ZLIB=OFF
                """
                sh "cmake --build '${WORKSPACE}/deps/ixwebsocket/.build'"
                sh "cmake --install '${WORKSPACE}/deps/ixwebsocket/.build'"
            }
            post {
                always {
                    sh "rm -rf '${WORKSPACE}/deps/ixwebsocket/.src' '${WORKSPACE}/deps/ixwebsocket/.build'"
                }
            }
        }

        stage('Build nlohmann/json') {
            steps {
                dir('deps/nlohmann-json/.src') {
                    checkout([
                        $class: 'GitSCM',
                        branches: [[name: "${NLOHMANN_JSON_COMMIT}"]],
                        userRemoteConfigs: [[url: 'https://github.com/nlohmann/json.git']]
                    ])
                }
                sh """
                    cmake -G Ninja -B "${WORKSPACE}/deps/nlohmann-json/.build" -S "${WORKSPACE}/deps/nlohmann-json/.src" \\
                        -DCMAKE_INSTALL_PREFIX="${WORKSPACE}/deps/nlohmann-json" \\
                        -DJSON_BuildTests=OFF \\
                        -DJSON_CI=OFF \\
                        -DJSON_DisableEnumSerialization=OFF \\
                        -DJSON_GlobalUDLs=OFF \\
                        -DJSON_ImplicitConversions=ON \\
                        -DJSON_Install=ON \\
                        -DJSON_LegacyDiscardedValueComparison=OFF \\
                        -DJSON_MultipleHeaders=ON \\
                        -DJSON_SystemInclude=OFF
                """
                sh "cmake --build '${WORKSPACE}/deps/nlohmann-json/.build'"
                sh "cmake --install '${WORKSPACE}/deps/nlohmann-json/.build'"
            }
            post {
                always {
                    sh "rm -rf '${WORKSPACE}/deps/nlohmann-json/.src' '${WORKSPACE}/deps/nlohmann-json/.build'"
                }
            }
        }

        stage('Configure roah-lib (release)') {
            steps {
                sh """
                    cmake -G Ninja -B "${WORKSPACE}/build" -S "${WORKSPACE}" \
                        -DCMAKE_BUILD_TYPE=Release \
                        -DCMAKE_INSTALL_PREFIX="${WORKSPACE}/install" \
                        -DCMAKE_PREFIX_PATH="${DEPS_PREFIX_PATH}" \
                        -DBUILD_DOCS=1 \
                        -DBUILD_TESTS=1
                """
            }
        }

        stage('Build roah-lib (release)') {
            steps {
                sh "cmake --build '${WORKSPACE}/build'"
            }
        }

        stage('Install roah-lib (release)') {
            steps {
                sh "cmake --install '${WORKSPACE}/build'"
            }
        }

        stage('Run tests (release)') {
            steps {
                sh "ctest --test-dir '${WORKSPACE}/build' --output-on-failure --output-junit '${WORKSPACE}/test-results-release.xml'"
            }
            post {
                always {
                    junit 'test-results-release.xml'
                }
            }
        }

        stage('Configure roah-lib (debug)') {
            steps {
                sh """
                    cmake -G Ninja -B "${WORKSPACE}/build_d" -S "${WORKSPACE}" \
                        -DCMAKE_BUILD_TYPE=Debug \
                        -DCMAKE_INSTALL_PREFIX="${WORKSPACE}/install_d" \
                        -DCMAKE_PREFIX_PATH="${DEPS_PREFIX_PATH}" \
                        -DBUILD_DOCS=1 \
                        -DBUILD_TESTS=1
                """
            }
        }

        stage('Build roah-lib (debug)') {
            steps {
                sh "cmake --build '${WORKSPACE}/build_d'"
            }
        }

        stage('Install roah-lib (debug)') {
            steps {
                sh "cmake --install '${WORKSPACE}/build_d'"
            }
        }

        stage('Run tests (debug)') {
            steps {
                sh "ctest --test-dir '${WORKSPACE}/build_d' --output-on-failure --output-junit '${WORKSPACE}/test-results-debug.xml'"
            }
            post {
                always {
                    junit 'test-results-debug.xml'
                }
            }
        }
    }
}
