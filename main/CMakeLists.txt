set(IDF_CCACHE_ENABLE 1)
idf_component_register(
  SRCS #
  "AppCamera.c" #
  "AppHttp.c" #
  "AppWifi.c" #
  "AppMain.c" #
  INCLUDE_DIRS #
  "." #
)
