# SolarisEngine

Solaris Engine, es un motor de videojuegos de la asignatura �Motors De Videojocs� en la universidad UPC-CITM : Dise�o y Desarollo de Videojuegos realizado durante el 3er a�o por: Mart� Sabater Miranda([@msabate00](https://github.com/msabate00).), Jia Jie Lin([@Linjiajie2002](https://github.com/Linjiajie2002)), Aleix Botella Llobet([@Aleix3](https://github.com/Aleix3)).

## Nuestro Motor
Solaris Engine es un motor en 3D realiado con SDL2, OpenGL, glew, DevIL, GLM, Assimp e ImGui. Programado en c y c++17.

### Implementaciones
- Cargar FBXs(Drag and drop).
- Cargar Texturas(PNG, JPG, JPEG, DDS).
- Gesti�n de GameObjects.
- 3 Componentes basicos para los GameObjects:
    -Transform: Permite ver y modificar la posicion, rotacion y escala.
    -Mesh: Permite dibujar la maya en la escena y muestra informacion de esta(normales, Bounding Box).
    -Material: Permite aplicar la textura al objeto asi como mostrar informacion de esta y cargar la textura de cuadros para comprobar las UVs.
- Camara del editor(Renderizado y Movimiento).
- Ventanas Del editor:
    -Menu Principal: Barra superior que permite ver estadisticas, editar preferencias, obtener mas informacion sobre el motor, cargar primitivos, etc.
    - Jerarquia: Muestra y permite seleccionar los objetos de la escena asi como eliminarlos y duplicarlos.
    - Inspector: Muestra y permite modificar los componentes del objeto seleccionado.
    - Consola: Muestra el proceso de carga de la geometria y de la inicializaci�n de las bibliotecas as� como los LOGs escritos por el usuario.
    - Proyecto: Muestra los archivos del proyecto dentro de la carpeta de "Assets".


### Controles: 
    C�mara:
        RMB: Rotar c�mara
        RMB+W: Avanzar c�mara
        RMB+S: Retoceder c�mara
        RMB+D: Mover hacia la derecha la c�mara
        RMB+A: Mover hacia la izquierda la c�mara
        LSHIFT: Duplica la velocidad del movimiento de la c�mara
        MMB Scroll: zoom in || zoom out
        MMB: Desplazamiento de la c�mara
        ALT+LMB: Orbitar el objeto seleccionado
        F: Centra la camara a objeto seleccionado






