QT += core
QT -= gui

CONFIG += c++11

TARGET = damn
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp

INCLUDEPATH += "C:\OpenCV3.3.0\opencv\build\include"

LIBS += -L"C:\\OpenCV3.3.0\\release3\\bin" \
    libopencv_core330 \
    libopencv_highgui330 \
    libopencv_imgproc330 \
    libopencv_features2d330 \
    libopencv_calib3d330 \
    libopencv_objdetect330 \
    libopencv_imgcodecs330 \
    libopencv_video330 \
    libopencv_videoio330 \

DISTFILES += \
    sagui.jpg
