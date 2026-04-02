pipeline {
    agent any

    environment {
        SPDLOG_COMMIT       = '79524ddd08a4ec981b7fea76afd08ee05f83755d'  # tag 1.7.0
        TOML11_COMMIT       = 'be08ba2be2a964edcdb3d3e3ea8d100abc26f286'  # tag v4.4.0
        GOOGLETEST_COMMIT   = '52eb8108c5bdec04579160ae17225d66034bd723'  # tag v1.17.0
    }

    stages {
        stage('Build spdlog') {
            steps {
                dir('${WORKSPACE}/deps/spdlog/.src') {
                    checkout([
                        $class: 'GitSCM',
                        branches: [[name: "${SPDLOG_COMMIT}"]],
                        userRemoteConfigs: [[url: 'https://github.com/gabime/spdlog.git']]
                    ])
                }
                sh """
                    cmake -B "${WORKSPACE}/deps/spdlog/.build" -S "${WORKSPACE}/deps/spdlog/.src" \
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
                sh "cmake --build '${WORKSPACE}/deps/spdlog/.build' --config Release"
                sh "cmake --install '${WORKSPACE}/deps/spdlog/.build' --config Release"
            }
            post {
                always {
                    sh "rm -rf '${WORKSPACE}/deps/spdlog/.src' '${WORKSPACE}/deps/spdlog/.build'"
                }
            }
        }

        stage('Build toml11') {
            steps {
                dir('${WORKSPACE}/deps/toml11/.src') {
                    checkout([
                        $class: 'GitSCM',
                        branches: [[name: "${TOML11_COMMIT}"]],
                        userRemoteConfigs: [[url: 'https://github.com/ToruNiina/toml11.git']]
                    ])
                }
                sh """
                    cmake -B "${WORKSPACE}/deps/toml11/.build" -S "${WORKSPACE}/deps/toml11/.src" \
                        -DCMAKE_INSTALL_PREFIX="${WORKSPACE}/deps/toml11" \
                        -Dtoml11_BUILD_TESTS=OFF
                """
                sh "cmake --build '${WORKSPACE}/deps/toml11/.build' --config Release"
                sh "cmake --install '${WORKSPACE}/deps/toml11/.build' --config Release"
            }
            post {
                always {
                    sh "rm -rf '${WORKSPACE}/deps/toml11/.src' '${WORKSPACE}/deps/toml11/.build'"
                }
            }
        }

        stage('Build googletest') {
            steps {
                dir('${WORKSPACE}/deps/googletest/.src') {
                    checkout([
                        $class: 'GitSCM',
                        branches: [[name: "${GOOGLETEST_COMMIT}"]],
                        userRemoteConfigs: [[url: 'https://github.com/google/googletest.git']]
                    ])
                }
                sh """
                    cmake -B "${WORKSPACE}/deps/googletest/.build" -S "${WORKSPACE}/deps/googletest/.src" \
                        -DCMAKE_INSTALL_PREFIX="${WORKSPACE}/deps/googletest" \
                        -DINSTALL_GTEST=1 \
                        -Dgtest_force_shared_crt=1 \
                        -DCMAKE_CXX_STANDARD=20 \
                        -DBUILD_SHARED_LIBS=OFF \
                        -DBUILD_GMOCK=OFF
                """
                sh "cmake --build '${WORKSPACE}/deps/googletest/.build' --config Release"
                sh "cmake --install '${WORKSPACE}/deps/googletest/.build' --config Release"
            }
            post {
                always {
                    sh "rm -rf '${WORKSPACE}/deps/googletest/.src' '${WORKSPACE}/deps/googletest/.build'"
                }
            }
        }
    }
}