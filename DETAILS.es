--Scintillator fiber 1.7

Versión final de la simulación del sistema Plástico/Fibra del SciCRT
por Rocio García
-Contiene una descripción detallada del plástico y la WLS.
-Modifique constante de Birks a 0.208 (materials) y DBL_MAX (detector construction)
-En el archivo macro (test.mac) las posiciones son (x,y,z). Coordenadas "z" positivas están más cerca del MAPMT, coordenadas negativas más lejos.
-Agregué un MAKEFILE para compilar.
-Utilizamos modo multithread.
 -Detectamos el número de núcleos del procesador.
-Jerarquía de los objetos corregida (fotones no se escapan de la fibra).
-Espectro de energía de EXPACS
  -Experimento para verificar la mínima energía que requiere un muon para cruzar el SciCRT ----Emin ~380Mev
-Cambia la estructura del los archivos de salida
-Incluye valor de reflectancia de la pintura en el borde de la fibra.
-Parametros ópticos leidos por medio de archivos.
-Descripción de la atenuación en la fibra correcta.
-Acoplamiento entre el coating y el centellador incluye efecto de rugosidad.
