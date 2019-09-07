Team Orienteering Problem - Técnica: Simulating Anealing.
Alejandro Vilches Cornejo 201573554-9

Instrucciones de Compilacion:
 - Tener el archivo de la instancia en la misma carpeta del programa.
 - Ejecutrar "make" para compliar.
 - Realizar "make run" para ejecutar el programa.
 * El resultado de muestra por consola, por lo que si desea mostrarlo en un archivo solo realize "make run >> archivo.txt"

Consideraciones:
  - Ingresar el nombre correcto del archivo el la linea indicada.
  - Para modificar los resultados obtenido para una misma instancia, debe variar los parámetros presentados al inicio del programa, sinedo estos:
    * max_iter -> cantidad de iteraciones antes del termino del algoritmo
    * Temp  -> temperatura inicial y actual.
    * Temp_min -> temperatura minima a la que llegará
    * var_Temp -> cuanto se reducira la temperatura cuando se deba.
    * max_iter_temp -> cantidad de iteraciones que se consideran antes de modificar la temperatura.
    * penalty -> penalizacion.
  
  *** De existir el caso de que para una instancia el programa se quede trabado y no termine, considere aumentar la penalizacion o modificar los valores de temperatura, ya que esto se produce
      debido a que con los parametros dados, la solución entra en zonas de diversificación de las cuales no logra salir, ya que intenta ingresar nodos nuevos pero ya no quedan mas, por lo que
      se queda buscando infinitamente nuevos nodos. ***
