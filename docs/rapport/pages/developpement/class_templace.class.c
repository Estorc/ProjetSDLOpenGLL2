class NomClasse : public SuperClasse {
    __containerType__ Type_du_this *
private: // Les attributs privés doivent être définis en premier
    static RessourceType ressourcePartagee;
    Donnee locale;
public:
    /**
     * @brief Constructeur / initialiseur de la classe.
     */
    void constructor();
    /**
     * @brief Méthode générique.
     */
    ReturnType methode(Type param);
    /**
     * @brief Méthode de rendu ou de calcul.
     */
    void render(Context ctx);
    /**
     * @brief Nettoyage ou libération des ressources.
     */
    void destroy();
};