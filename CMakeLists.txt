# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS "$ENV{IDF_PATH}/examples/common_components/protocol_examples_common"
						 "$ENV{IDF_PATH}/esp-aws-iot/libraries/backoffAlgorithm"
						 "$ENV{IDF_PATH}/esp-aws-iot/libraries/coreMQTT"
						 "$ENV{IDF_PATH}/esp-aws-iot/libraries/common/posix_compat"
						 "/components"
	)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(tls_mutual_auth)

target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/certs/root_cert_auth.pem" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/certs/client.crt" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/certs/client.key" TEXT)
