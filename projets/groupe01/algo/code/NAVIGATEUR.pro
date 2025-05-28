QT       += core gui widgets # Modules Qt de base, GUI et Widgets sont essentiels
QT       += webenginewidgets # Crucial pour QWebEngineView
RC_ICONS = C:/Users/Lenovo/OneDrive/Documenten/NAVIGATEUR/apps_web_browser_15742.ico

# Définit le nom de l'exécutable cible
TARGET = NAVIGATEUR

# Définit la norme C++ à C++17
CONFIG += c++17

# Spécifie la configuration de compilation en mode "release" (publication)
# Ceci assure que le projet compile pour le déploiement avec des optimisations.
CONFIG += release

# Fichiers d'entrée
SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Optionnel : Ajoute une icône d'application pour Windows.
# Assurez-vous que le chemin vers votre fichier .ico est correct.
# RC_ICONS = C:/Chemin/Vers/Votre/Icone/apps_web_browser_15742.ico

# Règles par défaut pour le déploiement (par défaut de Qt Creator, généralement suffisantes)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
