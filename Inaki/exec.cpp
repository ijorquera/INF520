#include <iostream>
#include <vector>

std::vector<int> r1;
std::vector<int> r2;

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
    // TODO: Agregar una funcion que indique le tamaño de valores que tiene el universo
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

void deRle(std::vector<int> &v){
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
    if(res2.size() < 16){
        res2.insert(res2.end(), 16-res2.size(), act);
    }
    std::cout << std::endl << "Vector deRLE obtenido: ";
    for (int i: res2){
        std::cout << i << ' ';   
    }
    std::cout << std::endl;
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
    // Proceso para RLE
    zorder(x2, 0, 0, 4, add2);
    std::cout << std::endl;
    std::cout << "Vector obtenido: ";
    for (int i: r2){
        std::cout << i << ' ';   
    }
    std::cout << std::endl;
    std::vector<int> rcomp = rle(r2);
    deRle(rcomp);
    return 0;
}