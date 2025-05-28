# ELibraryApp.pro

# Ce modèle spécifie le type de projet.
# 'app' signifie qu'il s'agit d'une application GUI standard.
TEMPLATE = app

# La variable CONFIG configure diverses options de compilation.
# 'debug' et 'release' activent les compilations standard de débogage/publication.
# 'qml_debug' est généralement pour les applications QML, pas strictement nécessaire ici mais inoffensif.
# 'c++17' spécifie la norme C++ à utiliser pour la compilation.
CONFIG += debug release qml_debug c++17

# La variable QT spécifie les modules Qt utilisés par votre projet.
# 'widgets' est essentiel pour les applications GUI utilisant QMainWindow, QPushButton, QTableWidget, etc.
# 'core' est fondamental pour les classes non-GUI, les boucles d'événements, QString, etc.
# 'gui' fournit les fonctionnalités GUI de base comme QIcon, QApplication, etc.
QT += widgets core gui
RC_ICONS = C:/Users/Lenovo/OneDrive/Documenten/ELibraryApp/Blackvariant-Button-Ui-System-Folders-Drives-Library.ico

# HEADERS spécifie tous les fichiers d'en-tête (.h) de votre projet.
# QMake exécutera automatiquement le compilateur de méta-objets (MOC) sur les en-têtes qui
# contiennent des macros Q_OBJECT (comme mainwindow.h et librarymanager.h).
HEADERS += \
    mainwindow.h \
    book.h \
    user.h \
    librarymanager.h

# SOURCES spécifie tous les fichiers source C++ (.cpp) de votre projet.
# Ces fichiers seront compilés en fichiers objets puis liés ensemble.
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    librarymanager.cpp

# FORMS spécifie tous les fichiers UI de Qt Designer (.ui) de votre projet.
# QMake exécutera automatiquement le compilateur d'interface utilisateur (UIC) sur ces fichiers
# pour générer les fichiers d'en-tête ui_*.h correspondants (par exemple, ui_mainwindow.h).
FORMS += \
    mainwindow.ui

# Facultatif : Fichier de ressources pour les éléments comme les icônes. Non utilisé dans ce projet,
# mais inclus comme un espace réservé courant.
# RESOURCES += \
#    resources.qrc

# Facultatif : Définir d'autres bibliothèques ou paramètres spécifiques à la plateforme ici.
# Par exemple, pour lier une bibliothèque externe :
# LIBS += -L/chemin/vers/ma/lib -lmylib

# Facultatif : Des étapes de compilation personnalisées peuvent être ajoutées.
# QMAKE_POST_LINK += $$PWD/copier_fichiers_donnees.bat

# Nom cible de l'exécutable. Par défaut, c'est le nom du projet.
TARGET = ELibraryApp

# Facultatif : Spécifier le répertoire de sortie pour l'exécutable.
# DESTDIR = $$PWD/bin

# Facultatif : Spécifier les drapeaux du compilateur pour une compilation plus stricte.
# QMAKE_CXXFLAGS += -Wall -Wextra
