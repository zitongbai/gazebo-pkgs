# gazebo-pkgs

A collection of tools and plugins for Gazebo.

Please also refer to [the wiki](https://github.com/JenniferBuehler/gazebo-pkgs/wiki) for more information.

# About this fork

Remove the unnecessary packages for gazebo grasp fix. 

Modify the CMakeLists.txt in pkg `gazebo_grasp_plugin`: 

Change from:
```CMakeLists
install(TARGETS gazebo_grasp_fix gazebo_grasp_msgs
  ARCHIVE DESTINATION lib/${PROJECT_NAME}
  LIBRARY DESTINATION lib/${PROJECT_NAME}
  RUNTIME DESTINATION bin/${PROJECT_NAME}
)
```
to
```CMakeLists
install(TARGETS gazebo_grasp_fix gazebo_grasp_msgs
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
```

otherwise you would get the error: 
```shell
CMake Error at /home/xxx/ur_project_ros2/install/gazebo_grasp_plugin/share/gazebo_grasp_plugin/cmake/ament_cmake_export_libraries-extras.cmake:48 (message):
  Package 'gazebo_grasp_plugin' exports the library 'gazebo_grasp_fix' which
  couldn't be found
Call Stack (most recent call first):
  /home/xxx/ur_project_ros2/install/gazebo_grasp_plugin/share/gazebo_grasp_plugin/cmake/gazebo_grasp_pluginConfig.cmake:41 (include)
  CMakeLists.txt:15 (find_package)
```