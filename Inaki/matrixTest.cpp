// g++ matrixTest.cpp -o test
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>

std::vector<int> r1;
std::vector<int> r2;

// Manejo para el proceso de arbol binario
struct node {
    int data;
    struct node* left;
    struct node* right;
};

template<typename M, typename CBACK>
void zorder(const M& m, int y0, int x0, int size,
            CBACK cback)
{
    if (size == 1) {
        // Base case, just one cell
        cback(m[y0][x0]);
    } else {
        // Recurse in Z-order
        int h = size/2;
        zorder(m, y0,   x0,   h, cback); // top-left
        zorder(m, y0,   x0+h, h, cback); // top-right
        zorder(m, y0+h, x0,   h, cback); // bottom-left
        zorder(m, y0+h, x0+h, h, cback); // bottom-right
    }
}

// Crea el conjunto de valores a partir del bit-vector
std::vector<int> obtenerValores(std::vector<int> &v){
    std::vector<int> res;
    for(int i = 0; i<v.size(); i++){
        if(v[i] == 1){
            res.push_back(i);
        }
    }

    return(res);
}

//Custom logarithms
double c_log(int base, int value){
    return log(value) / log(base);
}

void print(int x) {
    std::cout << x << " ";
}

void add(int x){
    ::r1.push_back(x);
    std::cout << x << " ";
}

void add2(int x){
    ::r2.push_back(x);
    std::cout << x << " ";
}

std::vector<int> gap(std::vector<int> &v){
    std::vector<int> res;
    int c = 0;
    for (int i: v){
        if(i == 0){
            c++;
        } else {
            res.push_back(c);
            c = 0;
        }
    }
    std::cout << "Vector GAP obtenido: ";
    for (int i: res){
        std::cout << i << ' ';   
    }
    std::cout << std::endl;
    return res;
}

void deGap(std::vector<int> &v){
    // Vector inicializado en 0
    // TODO: Agregar una funcion que indique el tamaño de valores que tiene el universo
    std::vector<int> res(16,0);
    int prevPos = -1;
    // Se recorre el vector para descomprimir y a la vez mostrar los valores que almacena
    std::cout << "Valores del conjunto: ";
    for (int i: v){
        // posicion actual + valor comprimido + 1
        prevPos += (i + 1);
        // Se muestra el valor parte del conjunto
        std::cout << prevPos << ' ';
        // Se cambia el valor en el resultado para mostrar el array binario descomprimido
        res[prevPos] = 1;
    }
    std::cout << std::endl << "Vector deGAP obtenido: ";
    for (int i: res){
        std::cout << i << ' ';   
    }
    std::cout << std::endl;
}

// Recibe el vector zorder y el tamaño n para obtener el indice de peor caso para GAP
void gapWC(std::vector<int> &v, int n){
    std::vector<int> res = obtenerValores(v);
    std::vector<double> to_sum;
    double temp = floor(log2(res.front() - 1.0));
    if(temp < 0 || std::isnan(temp)){
        to_sum.push_back(1.0);
    } else {
        to_sum.push_back( temp + 1.0);
    }
    for(int i = 1; i<res.size(); i++){
        temp = floor(log2(res[i]-res[i-1]-1.0));
        if( temp < 0 || std::isnan(temp) ){
            to_sum.push_back( 1.0 );
        } else {
            to_sum.push_back( temp + 1.0 );
        }
    }

    double result = 0;
    for(int i = 0; i<to_sum.size(); i++){
        result = result + to_sum[i];
    }
    std::cout << "Medida de peor caso GAP: " << result << std::endl;
    return;
}

// Recibe el vector con los gaps para calcular H_0(GAP)
void h_0gap(std::vector<int> &v){
    int n = v.size();
    int act;
    int med = -1;
    int cont = 0;
    std::vector<int> temp;
    sort(v.begin(), v.end());
    std::vector<std::vector<int>> vect;
    // Proceso la distribucion de probabilidad de cada numero, al estar ordenado podre contar la cant de veces de cada numero y agregarlas a un vector de vectores
    // Cada sub vector contiene la dupla, valor - cantidad, para poder procesar y obtener el H_0
    for(int i = 0; i<v.size(); i++){
        act = v[i];
        if(med == -1){
            med = act;
        }
        if(act == med){
            cont++;
        } else {
            temp = { med, cont };
            vect.push_back(temp);
            cont = 1;
            med = act;
        }
    }
    temp = { med, cont };
    vect.push_back(temp);

    // Se almacenara en un vector los valores para luego aplicar la sumatoria a este
    std::vector<double> to_sum;
    // Para corroborar calculos
    double temp2;
    double t;
    // Se itera por cada valor del vector de vectores
    for(int i = 0; i<vect.size(); i++){
        t = vect[i][1];
        temp2 = log2(n/vect[i][1]);
        if(temp2 < 0){
            to_sum.push_back( t/n );
        } else {
            to_sum.push_back( (t/n)*temp2 );
        }
    }
    double result = 0;
    for(int i = 0; i<to_sum.size(); i++){
        result = result + to_sum[i];
    }
    std::cout << "Medida de H_0(GAP): " << result << std::endl;
    return;
}

std::vector<int> rle(std::vector<int> &v){
    std::vector<int> res;
    int c0 = 0;
    int c1 = 0;
    if(v[0] == 0){
        res.push_back(0);
    } else {
        res.push_back(1);
    }
    for (int i: v){
        if(i == 0){
            c0++;
            if(c1 != 0){
                res.push_back(c1);
                c1 = 0;
            }
        } else {
            c1++;
            if(c0 != 0){
                res.push_back(c0);
                c0 = 0;
            }
        }
    }
    std::cout << "Vector RLE obtenido: ";
    for (int i: res){
        std::cout << i << ' ';   
    }
    std::cout << std::endl;
    return res;
}

void deRle(std::vector<int> &v, int n){
    // Vector inicializado
    std::vector<int> res2 = {};
    // TODO: Saber cual es n^2 para el valor final
    // Se obtiene el primer valor para saber con cual inicia
    int act = v.front();
    // Se elimina el primer valor para no confundir al sistema
    v.erase(v.begin());
    // Se recorre el resto vector para descomprimir
    for (int i: v){
        // Agrego X veces el valor de i
        res2.insert(res2.end(), i, act);
        // Realizo un flip al valor para pasar de 1 a 0
        act = 1 - act;
    }
    // Finalmente se agrega la cantidad final para terminar el vector
    if(res2.size() < n){
        res2.insert(res2.end(), n-res2.size(), act);
    }
    std::cout << "Vector deRLE obtenido: ";
    for (int i: res2){
        std::cout << i << ' ';   
    }
    std::cout << std::endl;
}

// Calculo de peor caso para RLE
// Recibe el vector comprimido con los valores 0 y 1
void rleWC(std::vector<int> &v){
    int act = v.front();
    std::vector<int> ceros;
    std::vector<int> unos;
    std::vector<double> to_sum_ceros;
    std::vector<double> to_sum_unos;
    double temp;
    for (int i = 1; i<v.size(); i++){
        // Se agrega la cantidad de 1 y 0 en el arreglo
        if(act == 0){
            ceros.push_back(v[i]);
        } else {
            unos.push_back(v[i]);
        }
        act = 1-act;
    }
    for (int i = 0; i<ceros.size(); i++){
        temp = floor(log2(ceros[i]-1.0));
        if( temp < 0 || std::isnan(temp) ){
            to_sum_ceros.push_back( 1.0 );
        } else {
            to_sum_ceros.push_back( temp + 1.0 );
        }
    }
    for (int i = 0; i<unos.size(); i++){
        temp = floor(log2(unos[i]-1.0));
        if( temp < 0 || std::isnan(temp) ){
            to_sum_unos.push_back( 1.0 );
        } else {
            to_sum_unos.push_back( temp + 1.0 );
        }
    }
    double result1 = 0;
    for(int i = 0; i<to_sum_ceros.size(); i++){
        result1 = result1 + to_sum_ceros[i];
    }
    double result2 = 0;
    for(int i = 0; i<to_sum_unos.size(); i++){
        result1 = result1 + to_sum_unos[i];
    }
    std::cout << "Medida de peor caso RLE: " << result1+result2+1 << std::endl;
}

// Recibe el vector con los RLE para calcular H_0(RLE)
void h_0rle(std::vector<int> &v){
    // Obtener por separado la cantidad de ceros y unos
    int act = v.front();
    std::vector<int> ceros;
    std::vector<int> unos;
    double temp;
    for (int i = 1; i<v.size(); i++){
        // Se agrega la cantidad de 1 y 0 en el arreglo
        if(act == 0){
            ceros.push_back(v[i]);
        } else {
            unos.push_back(v[i]);
        }
        act = 1-act;
    }
    // Para cada vector se repite lo realizado con H_0(GAP)
    int n = ceros.size();
    int act1;
    int med = -1;
    int cont = 0;
    std::vector<int> temp1;
    sort(ceros.begin(), ceros.end());
    std::vector<std::vector<int>> vect;
    // Proceso la distribucion de probabilidad de cada numero, al estar ordenado podre contar la cant de veces de cada numero y agregarlas a un vector de vectores
    // Cada sub vector contiene la dupla, valor - cantidad, para poder procesar y obtener el H_0
    for(int i = 0; i<ceros.size(); i++){
        act1 = ceros[i];
        if(med == -1){
            med = act1;
        }
        if(act1 == med){
            cont++;
        } else {
            temp1 = { med, cont };
            vect.push_back(temp1);
            cont = 1;
            med = act1;
        }
    }
    temp1 = { med, cont };
    vect.push_back(temp1);

    // Para los unos
    int n2 = unos.size();
    int act2;
    int med2 = -1;
    int cont2 = 0;
    std::vector<int> temp2;
    sort(unos.begin(), unos.end());
    std::vector<std::vector<int>> vect2;
    // Proceso la distribucion de probabilidad de cada numero, al estar ordenado podre contar la cant de veces de cada numero y agregarlas a un vector de vectores
    // Cada sub vector contiene la dupla, valor - cantidad, para poder procesar y obtener el H_0
    for(int i = 0; i<unos.size(); i++){
        act2 = unos[i];
        if(med2 == -1){
            med2 = act2;
        }
        if(act2 == med2){
            cont2++;
        } else {
            temp2 = { med2, cont2 };
            vect2.push_back(temp2);
            cont2 = 1;
            med2 = act2;
        }
    }
    temp2 = { med2, cont2 };
    vect2.push_back(temp2);

    // Ahora por cada cantidad procesada de ceros y unos, se obtienen sus H_0 por separado y al final se suman
    // Se almacenara en un vector los valores para luego aplicar la sumatoria a este
    std::vector<double> to_sum_ceros;
    // Para corroborar calculos
    double temp3;
    double t;
    // Se itera por cada valor del vector de vectores
    for(int i = 0; i<vect.size(); i++){
        t = vect[i][1];
        temp3 = log2(n/vect[i][1]);
        if(temp3 < 0){
            to_sum_ceros.push_back( t/n );
        } else {
            to_sum_ceros.push_back( (t/n)*temp3 );
        }
    }

    std::vector<double> to_sum_unos;
    // Para corroborar calculos
    double temp4;
    double t2;
    // Se itera por cada valor del vector de vectores
    for(int i = 0; i<vect2.size(); i++){
        t2 = vect2[i][1];
        temp4 = log2(n2/vect2[i][1]);
        if(temp4 < 0){
            to_sum_unos.push_back( t2/n2 );
        } else {
            to_sum_unos.push_back( (t2/n2)*temp4 );
        }
    }

    double result1 = 0;
    for(int i = 0; i<to_sum_ceros.size(); i++){
        result1 = result1 + to_sum_ceros[i];
    }
    double result2 = 0;
    for(int i = 0; i<to_sum_unos.size(); i++){
        result2 = result2 + to_sum_unos[i];
    }

    std::cout << "Medida de H_0(RLE): " << result1+result2 << std::endl;

    return;
}

// Paso para la conversion de 2 enteros a z order
std::vector<int> obtenerBits(int numero) {
    std::vector<int> bits;
    // Obtener los bits del número
    while (numero > 0) {
        bits.push_back(numero % 2);
        numero /= 2;
    }
    // Invertir el orden de los bits en el vector
    std::reverse(bits.begin(), bits.end());
    return bits;
}

// Creacion de nodos en arbol binario
struct node *newNode(int data) {
    struct node *node = (struct node *)malloc(sizeof(struct node));

    node->data = data;
    node->left = NULL;
    node->right= NULL;
    return(node);
}

// V: Sera el vector con los numeros del conjunto ordenados de menor a mayor
// n: Es el universo del conjunto
struct node *Trie(std::vector<int> &v, int n){
    std::vector<int> act;
    struct node *root = newNode(-1);
    for (int i: v){
        // Por cada valor del conjunto, obtengo los bits a los que corresponde y se van agregando al arbol
        // 1- Obtener los bits
        act = obtenerBits(i);
        // 2- Reviso si el valor tiene al cantidad de bits correspondientes al conjunto
        if(act.size() < n){
            act.insert( act.begin(), n - act.size(), 0);
        }
        // 3- Itero por los bits del numero actual para ir armando el arbol
        struct node *temp = root;
        for (int a: act){
            // 0 = Left ; 1 = Right
            if(a == 0){
                // Si dicha rama del arbol no tiene creado el nodo todavia, lo creo
                if(temp->left == NULL){
                    temp->left = newNode(0);
                    temp = temp->left;
                // Si la tiene creada, entonces me muevo directamente.
                } else {
                    temp = temp->left;
                }
            } else {
                // Si dicha rama del arbol no tiene creado el nodo todavia, lo creo
                if(temp->right == NULL){
                    temp->right = newNode(1);
                    temp = temp->right;
                // Si la tiene creada, entonces me muevo directamente.
                } else {
                    temp = temp->right;
                }
            }
        }
    }
    return(root);
}

// Recorrido Preorder
void recorridoPreOrder(struct node *temp) {
  if (temp != NULL) {
    std::cout << " " << temp->data;
    recorridoPreOrder(temp->left);
    recorridoPreOrder(temp->right);
  }
}

// Peor caso en un arbol Trie
int count(struct node *root){
    if(root == NULL){
        return 0;
    }
    else{
        return 1 + count(root->left) + count(root->right);
    }
}

// Funcion que procesa los valores para obtener el array en z-order
std::vector<int> procesarInput(int numero1, int numero2, int n_square){
    std::vector<int> bits1 = obtenerBits(numero1);
    std::vector<int> bits2 = obtenerBits(numero2);
    
    std::vector<int> res;

    if(bits1.size() < n_square){
        bits1.insert( bits1.begin(), n_square - bits1.size(), 0);
    }
    if(bits2.size() < n_square){
        bits2.insert( bits2.begin(), n_square - bits2.size(), 0);
    }

    // Ahora que estan ambos del mismo tamaño, se itera entre uno y otro para retornar el valor del vector en zorder.
    for(int i = 0; i<bits1.size(); i++){
        res.push_back(bits1[i]);
        res.push_back(bits2[i]);
    }
    
    return res;
}

int main(int argc, const char *argv[]) {
    int x[][4] = {{ 0,  0,  0,  0},
                  { 0,  1,  1,  1},
                  { 0,  0,  0,  1},
                  { 1,  0,  0,  0}};
    
    int x2[][4] ={{ 0,  1,  0,  1},
                  { 0,  1,  1,  1},
                  { 1,  1,  1,  1},
                  { 0,  0,  1,  0}};

    // Proceso para GAP
    zorder(x, 0, 0, 4, add);
    std::cout << std::endl;
    std::cout << "Vector obtenido: ";
    for (int i: r1){
        std::cout << i << ' ';   
    }
    std::cout << std::endl;
    std::vector<int> gcomp = gap(r1);
    deGap(gcomp);
    gapWC(r1, 16);
    std::vector<int> gap_test = {3,2,0,2,2};
    h_0gap(gap_test);


    // Proceso para RLE
    std::cout << std::endl << std::endl;
    zorder(x2, 0, 0, 4, add2);
    std::cout << std::endl << "Vector obtenido: ";
    for (int i: r2){
        std::cout << i << ' ';   
    }
    std::cout << std::endl;
    std::vector<int> rcomp = rle(r2);
    std::vector<int> rcomp2 = rle(r2);
    deRle(rcomp, 16);
    rleWC(rcomp2);

    // TEST de BITS
    std::vector<int> bits = procesarInput(2,1,3);
    std::cout << std::endl << std::endl;
    std::cout << "Prueba de input para 2 enteros a Z-Order:" << std::endl;
    for (int i: bits){
        std::cout << i << ' ';   
    }
    std::cout << std::endl << std::endl;
    

    //TEST para TRIE
    std::vector<int> arbol = {4, 5, 7, 9, 16, 17, 20, 23, 32, 33, 47, 208, 218};
    struct node *tree = Trie(arbol, 8);
    std::cout << "Recorrido PreOrder: " << std::endl;
    recorridoPreOrder(tree);
    std::cout << std::endl << "Valor esperado: " << std::endl;
    std::cout << " -1 0 0 0 1 1 1 1 0 0" << std::endl;
    std::cout << "Medida de peor caso: " << count(tree)-1 << std::endl;



    return 0;
}