TEMPLATE = app
CONFIG += console c++11 c++14
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

SOURCES += main.cpp \
    Food.cpp \
    Genetic.cpp

HEADERS += \
    Nutrition.h \
    Food.h \
    NutritionError.h \
    Nutrient.h \
    Genetic.h \
    FoodContainer.h
