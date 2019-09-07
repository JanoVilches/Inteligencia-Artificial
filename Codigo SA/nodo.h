//Class NODO: sera la que guarde la info de la posición (x,y) y el score. 
class NODO
{
public:
    float posicion[2];
    int score;
    int visitado;//0:no visitado, 1:visitado
    int numero;

public:
    NODO(float x, float y, int s, int n); //Constructor
    ~NODO(); //Destructor
};

NODO::NODO(float x, float y, int s, int n)
{
    posicion[0]=x;
    posicion[1]=y;
    score = s;
    visitado = 0;
    numero = n;
}

NODO::~NODO()
{
}