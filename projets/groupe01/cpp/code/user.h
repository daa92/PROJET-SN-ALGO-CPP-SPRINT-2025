// user.h
#ifndef USER_H
#define USER_H

#include <QString>
#include <QUuid>       // Pour générer des identifiants utilisateur uniques
#include <QStringList> // Nécessaire pour la définition de QStringList et QList<QString>

/**
 * @brief La classe User représente un usager de la bibliothèque (étudiant ou enseignant).
 * Elle stocke les détails personnels et un identifiant unique.
 */
class User {
public:
    QString id;            ///< Identifiant unique de l'utilisateur
    QString name;          ///< Nom complet de l'utilisateur
    QString phoneNumber;   ///< Numéro de téléphone de l'utilisateur
    QString gmailAddress;  ///< Adresse Gmail de l'utilisateur

    /**
     * @brief Constructeur de la classe User.
     * Génère un identifiant unique pour chaque nouvel utilisateur.
     * @param name Nom de l'utilisateur.
     * @param phoneNumber Numéro de téléphone de l'utilisateur.
     * @param gmailAddress Adresse Gmail de l'utilisateur.
     */
    User(const QString& name = "", const QString& phoneNumber = "", const QString& gmailAddress = "")
        : name(name), phoneNumber(phoneNumber), gmailAddress(gmailAddress) {
        // Générer un identifiant unique en utilisant QUuid
        id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }

    /**
     * @brief Convertit les données de l'objet User en une chaîne de caractères séparée par des pipes.
     * Ce format est utilisé pour sauvegarder les données utilisateur dans un fichier.
     * @return Une représentation QString des données de l'utilisateur.
     */
    QString toString() const {
        return QString("%1|%2|%3|%4")
        .arg(id)
            .arg(name)
            .arg(phoneNumber)
            .arg(gmailAddress);
    }

    /**
     * @brief Méthode statique pour créer un objet User à partir d'une chaîne de caractères séparée par des pipes.
     * Ceci est utilisé pour charger les données utilisateur depuis un fichier.
     * @param data La chaîne de caractères contenant les données de l'utilisateur.
     * @return Un objet User rempli avec les données analysées.
     */
    static User fromString(const QString& data) {
        QStringList parts = data.split('|');
        if (parts.size() == 4) {
            User user; // Créer un objet User par défaut
            user.id = parts[0];
            user.name = parts[1];
            user.phoneNumber = parts[2];
            user.gmailAddress = parts[3];
            return user;
        }
        return User(); // Retourner un User par défaut (vide) si l'analyse échoue
    }
};

#endif // USER_H
