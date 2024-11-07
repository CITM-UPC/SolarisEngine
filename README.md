# SolarisEngine

Solaris Engine, es un motor de videojuegos de la asignatura “Motors De Videojocs” en la universidad UPC-CITM : Diseño y Desarrollo de Videojuegos, realizado durante el 3.º año por: Martí Sabaté Miranda([@msabate00](https://github.com/msabate00).), Jiajie Lin([@Linjiajie2002](https://github.com/Linjiajie2002)), Aleix Botella Llobet([@Aleix3](https://github.com/Aleix3)).

## Nuestro Motor
Solaris Engine es un motor en 3D realizado con SDL2, OpenGL, glew, DevIL, GLM, Assimp e ImGui. Programado en c y c++17.

### Implementaciones
- Cargar FBXs(Drag and drop).
- Cargar Texturas(PNG, JPG, JPEG, DDS).
- Gestión de GameObjects.
- 3 Componentes básicos para los GameObjects:
    -Transform: Permite ver y modificar la posición, rotación y escala.
    -Mesh: Permite dibujar la maya en la escena y muestra información de esta(normales, Bounding Box).
    -Material: Permite aplicar la textura al objeto así como mostrar información de esta y cargar la textura de cuadros para comprobar las UVs.
- Cámara del editor(Renderizado y Movimiento).
- Ventanas Del editor:
    -Menú Principal: Barra superior que permite ver estadísticas, editar preferencias, obtener más información sobre el motor, cargar primitivos, etc.
    - Jerarquía: Muestra y permite seleccionar los objetos de la escena así como eliminarlos y duplicarlos.
    - Inspector: Muestra y permite modificar los componentes del objeto seleccionado.
    - Consola: Muestra el proceso de carga de la geometría y de la inicialización de las bibliotecas, así como los LOGs escritos por el usuario.
    - Proyecto: Muestra los archivos del proyecto dentro de la carpeta de "Assets".


### Controles: 
- Cámara:
    - RMB: Rotar cámara
    - RMB+W: Avanzar cámara
    - RMB+S: Retroceder cámara
    - RMB+D: Mover hacia la derecha la cámara
    - RMB+A: Mover hacia la izquierda la cámara
    - LSHIFT: Duplica la velocidad del movimiento de la cámara
    - MMB Scroll: zoom in || zoom out
    - MMB: Desplazamiento de la cámara
    - ALT+LMB: Orbitar el objeto seleccionado
    - F: Centra la cámara a objeto seleccionado





