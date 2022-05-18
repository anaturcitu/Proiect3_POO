#include <iostream>
using namespace std;

///-----------------------clasa si functie template:-----------------------
template <class T, int n>
class creareVector
{
public:
    creareVector()
    {
        vector = new T[n]; // alocam spatiu pentru vector
    }

    int GetDim() { return n; } // functie inline pentru aflarea dimensiunii
    T& operator[] (int i); // operator pentru accesarea elementelor

    ~creareVector()
    {
        delete [] vector;
    }
private:
    creareVector operator=(creareVector&);
    creareVector(const creareVector&);

    T *vector; // elementele vectorului (de tip T)
};

template<class T, int n>
T& creareVector<T,n>::operator[] (int i)
{
    return vector[i];
}

template <class T>
void sortare(T vector[], int n)
{
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (vector[i] > vector[j])
            {
                T aux = vector[i];
                vector[i] = vector[j];
                vector[j] = aux;
            }
}

///-----------------------abstract factory:-----------------------
class Regizor
{
private:
    string nume;
public:
    Regizor(string n):nume(n){};
    string getNume(){return nume;}
};

class RegizorSf : public Regizor
{
public:
    RegizorSf():Regizor("RegizorSf") {}
};

class RegizorActiune : public Regizor
{
public:
    RegizorActiune():Regizor("RegizorActiune") {}
};

class Distributie
{
private:
    int numarActori;
public:
    Distributie(int numarActori):numarActori(numarActori) {}
    int getNumarActori() {return numarActori;}
};

class DistributieSf : public Distributie
{
public:
    DistributieSf():Distributie(150) {}
};

class DistributieActiune : public Distributie
{
public:
    DistributieActiune():Distributie(100) {}
};

class Film
{
private:
    string nume;
    Regizor *regizor;
    Distributie *distributie;
public:
    Film(string n):nume(n) {}
    void setRegizor(Regizor* r) { regizor = r;}
    void setDistributie(Distributie* d) {	distributie = d;}
    void afiseazaFilm()
    {
        cout << endl << "Film: " << nume << "\n";
        cout << "Regizor: " << regizor->getNume() << "\n";
        cout << "Distributie: " << distributie->getNumarActori() << "\n";
    }

    Film(string nume, Regizor *regizor, Distributie *distributie);
    Film(const Film&);
    Film();
    ~Film();

    string getNume() const {return nume;}
    Regizor getRegizor() const {return *regizor;}
    Distributie getDistributie() const {return *distributie;}
    bool operator>(Film&);
};


class FilmFactory
{
private:
    Film *film;
protected:
    virtual Regizor * buildRegizor() = 0;
    virtual Distributie * buildDistributie() = 0;
public:
    virtual Film* buildFilmIntreg() = 0;
};

class FilmSfFactory : public FilmFactory
{
    Regizor * buildRegizor()
    {
        return new RegizorSf();
    }
    Distributie * buildDistributie()
    {
        return new DistributieSf();
    }

    Film * buildFilmIntreg()
    {
        Film *film = new Film("FilmSf");
        film->setRegizor(buildRegizor());
        film->setDistributie(buildDistributie());
        return film;
    }
};

class FilmActiuneFactory : public FilmFactory
{
    Regizor *buildRegizor()
    {
        return new RegizorActiune();
    }

    Distributie *buildDistributie()
    {
        return new DistributieActiune();
    }

    Film *buildFilmIntreg()
    {
        Film *film = new Film("FilmActiune");
        film->setRegizor(buildRegizor());
        film->setDistributie(buildDistributie());
        return film;
    }
};

///-----------------------constructori / destructor pentru clasa Film:-----------------------
Film::Film(string nume, Regizor *regizor, Distributie *distributie)
{
    this->nume = nume;
    this->regizor = regizor;
    this->distributie = distributie;
}

Film::Film(const Film& film)
{
    nume = film.nume;
    regizor = film.regizor;
    distributie = film.distributie;
}

Film::Film(){}

Film::~Film() {}

///-----------------------supraincarcarea operatorilor:-----------------------
ostream& operator << (ostream& out, const Film& filme)
{
    out << filme.getNume();
    return out;
}

bool Film::operator>(Film& f2)
{
    return (this->nume > f2.nume);
}

///-----------------------singleton:-----------------------
class CaracteristiciFilm
 {
    static CaracteristiciFilm* instance;
    string NumeFilm;
    double Rating;
    string ActorPrincipal;

    CaracteristiciFilm() : NumeFilm("Doctor Strange"), Rating(8), ActorPrincipal("Benedict Cumberbatch"){}

public:
    static CaracteristiciFilm* getInstace()
    {
        if(instance == NULL)
            instance = new CaracteristiciFilm();
        return instance;
    }
    void setNumeFilm(string numeFilm) {NumeFilm = numeFilm;}
    void setRating(double rating) {Rating = rating;}
    void setActorPrincipal(string actor_principal) {ActorPrincipal = actor_principal;}

    string getNumeFilm() {return NumeFilm;}
    double getRating() {return Rating;}
    string getActorPrincipal() {return ActorPrincipal;}

    void displaySetting()
    {
        cout << "Nume film: " << NumeFilm << "\n";
        cout << "Rating: " << Rating << "\n";
        cout << "Actor principal: " << ActorPrincipal << "\n\n";
    }
};

CaracteristiciFilm * CaracteristiciFilm::instance = NULL;

void functie ()
{
    CaracteristiciFilm *setting = CaracteristiciFilm::getInstace();
    setting->displaySetting();
}

int main()
{
    /// template:
    double v0[] = {5.4, 3.7, 53, 2};
    sortare(v0, 4);
    cout << "\nSortare numere in ordine crescatoare si afisare:\n";
    for (int i = 0; i < 4; i++)
        cout << v0[i] << ' ';

    cout << "\n\n";
    Film f[3];
    f[0] = Film("Pirates of the Caribbean");
    f[1] = Film("Avatar");
    f[2] = Film("Interstellar");
    sortare<Film>(f, 3);

    cout << "Sortare filme in ordine alfabetica si afisare:\n";
    for (int i = 0; i < 3; i++)
        cout << f[i] << "\n";

    /// abstract factory:
    FilmFactory* factory = new FilmSfFactory;
    FilmFactory* factory1 = new FilmActiuneFactory;

    Film *film = factory->buildFilmIntreg();
    film->afiseazaFilm();

    Film *film1 = factory1->buildFilmIntreg();
    film1->afiseazaFilm();
    cout << "\n";

    /// singleton:
    CaracteristiciFilm *setting = CaracteristiciFilm::getInstace();
    setting->displaySetting();
    setting->setRating(9.5);
    functie();

    return 0;
}
