# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/POS/Pong_SemPraca_2/cmake-build-debug-vs/_deps/raylib-src"
  "C:/POS/Pong_SemPraca_2/cmake-build-debug-vs/_deps/raylib-build"
  "C:/POS/Pong_SemPraca_2/cmake-build-debug-vs/_deps/raylib-subbuild/raylib-populate-prefix"
  "C:/POS/Pong_SemPraca_2/cmake-build-debug-vs/_deps/raylib-subbuild/raylib-populate-prefix/tmp"
  "C:/POS/Pong_SemPraca_2/cmake-build-debug-vs/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
  "C:/POS/Pong_SemPraca_2/cmake-build-debug-vs/_deps/raylib-subbuild/raylib-populate-prefix/src"
  "C:/POS/Pong_SemPraca_2/cmake-build-debug-vs/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/POS/Pong_SemPraca_2/cmake-build-debug-vs/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/POS/Pong_SemPraca_2/cmake-build-debug-vs/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
