# Install script for directory: E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/file_server")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aesni.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/arc4.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/blowfish.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bn_mul.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/certs.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher_internal.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/compat-1.3.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp_internal.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy_poll.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/havege.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md2.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md4.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md_internal.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/padlock.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk_internal.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs11.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa_internal.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_internal.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    "E:/MyStuff/MyWork/ESP32_WROOVER/esp-idf/components/mbedtls/mbedtls/include/mbedtls/xtea.h"
    )
endif()

