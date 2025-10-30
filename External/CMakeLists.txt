set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
if(NOT TARGET imgui)
  set(IMGUI_FILES
      "${IMGUI_DIR}/imgui.cpp"
      "${IMGUI_DIR}/imgui_demo.cpp"
      "${IMGUI_DIR}/imgui_draw.cpp"
      "${IMGUI_DIR}/imgui_tables.cpp"
      "${IMGUI_DIR}/imgui_widgets.cpp"
      "${IMGUI_DIR}/imconfig.h"
      "${IMGUI_DIR}/imgui.h"
      "${IMGUI_DIR}/imgui_internal.h"
      "${IMGUI_DIR}/imstb_rectpack.h"
      "${IMGUI_DIR}/imstb_textedit.h"
      "${IMGUI_DIR}/imstb_truetype.h"
  )

  add_library(imgui STATIC ${IMGUI_FILES})
  target_include_directories(imgui PUBLIC 
    ${IMGUI_DIR} # for those, such as implot, including as <imgui.h>
    ${CMAKE_CURRENT_SOURCE_DIR}) # for those including as <imgui/imgui.h>

  set_property(TARGET imgui PROPERTY FOLDER "External")
endif()