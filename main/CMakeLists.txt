set(COMPONENT_SRCS
	"mqtt_mutual_auth.c"
	"tallyio-espidf.c"
	"sniffer.c"
	)

set(COMPONENT_ADD_INCLUDEDIRS
	"."
	"${CMAKE_CURRENT_LIST_DIR}"
	)

idf_component_register(SRCS "wifi_scanner.c" "${COMPONENT_SRCS}"
					   INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
					  )