# SolarisEngine

Solaris Engine, es un motor de videojuegos de la asignatura “Motors De Videojocs” en la universidad UPC-CITM : Diseño y Desarollo de Videojuegos realizado durante el 3er año por: Martí Sabater Miranda([@msabate00](https://github.com/msabate00).), Jia Jie Lin([@Linjiajie2002](https://github.com/Linjiajie2002)), Aleix Botella Llobet([@Aleix3](https://github.com/Aleix3)).

## Nuestro Motor
Solaris Engine es un motor en 3D realiado con SDL2, OpenGL, glew, DevIL, GLM, Assimp e ImGui. Programado en c y c++17.

### Implementaciones
- Cargar FBXs(Drag and drop).
- Cargar Texturas(PNG, JPG, JPEG, DDS).
- Gestión de GameObjects.
- 3 Componentes basicos para los GameObjects:
    -Transform: Permite ver y modificar la posicion, rotacion y escala.
    -Mesh: Permite dibujar la maya en la escena y muestra informacion de esta(normales, Bounding Box).
    -Material: Permite aplicar la textura al objeto asi como mostrar informacion de esta y cargar la textura de cuadros para comprobar las UVs.
- Camara del editor(Renderizado y Movimiento).
- Ventanas Del editor:
    -Menu Principal: Barra superior que permite ver estadisticas, editar preferencias, obtener mas informacion sobre el motor, cargar primitivos, etc.
    - Jerarquia: Muestra y permite seleccionar los objetos de la escena asi como eliminarlos y duplicarlos.
    - Inspector: Muestra y permite modificar los componentes del objeto seleccionado.
    - Consola: Muestra el proceso de carga de la geometria y de la inicialización de las bibliotecas así como los LOGs escritos por el usuario.
    - Proyecto: Muestra los archivos del proyecto dentro de la carpeta de "Assets".


### Controles: 
    Cámara:
        RMB: Rotar cámara
        RMB+W: Avanzar cámara
        RMB+S: Retoceder cámara
        RMB+D: Mover hacia la derecha la cámara
        RMB+A: Mover hacia la izquierda la cámara
        LSHIFT: Duplica la velocidad del movimiento de la cámara
        MMB Scroll: zoom in || zoom out
        MMB: Desplazamiento de la cámara
        ALT+LMB: Orbitar el objeto seleccionado
        F: Centra la camara a objeto seleccionado






