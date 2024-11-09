# SolarisEngine

Solaris Engine es un motor de videojuegos desarrollado para la asignatura “Motors De Videojocs” en la Universidad UPC-CITM: Diseño y Desarrollo de Videojuegos, realizado durante el 3.º año por: Martí Sabaté Miranda([@msabate00](https://github.com/msabate00)), Jiajie Lin([@Linjiajie2002](https://github.com/Linjiajie2002)), Aleix Botella Llobet([@Aleix3](https://github.com/Aleix3)).

## Nuestro Motor
Solaris Engine es un motor 3D desarrollado con SDL2, OpenGL, glew, DevIL, GLM, Assimp e ImGui, programado en C y C++17.

### Implementaciones
- Carga de FBXs (arrastrar y soltar).
- Carga de texturas (PNG, JPG, JPEG, DDS).
- Gestión de GameObjects.
- Tres componentes básicos para los GameObjects:
    - **Transform**: Permite ver y modificar la posición, rotación y escala.
    - **Mesh**: Permite dibujar la malla en la escena y muestra información de esta (normales, Bounding Box).
    - **Material**: Permite aplicar la textura al objeto, mostrar información de esta y cargar la textura de cuadros para comprobar las UVs.
- Cámara del editor (renderizado y movimiento).
- Ventanas del editor:
    - **Menú Principal**: Barra superior que permite ver estadísticas, editar preferencias, obtener más información sobre el motor, cargar primitivas, etc.
    - **Jerarquía**: Muestra y permite seleccionar los objetos de la escena, así como eliminarlos y duplicarlos.
    - **Inspector**: Muestra y permite modificar los componentes del objeto seleccionado.
    - **Consola**: Muestra el proceso de carga de la geometría y de la inicialización de las bibliotecas, así como los logs escritos por el usuario.
    - **Proyecto**: Muestra los archivos del proyecto dentro de la carpeta de "Assets".

### Controles: 
- Cámara:
    - **RMB**: Rotar cámara
    - **RMB + W**: Avanzar cámara
    - **RMB + S**: Retroceder cámara
    - **RMB + D**: Mover la cámara hacia la derecha
    - **RMB + A**: Mover la cámara hacia la izquierda
    - **LSHIFT**: Duplica la velocidad de movimiento de la cámara
    - **MMB Scroll**: Zoom in / Zoom out
    - **MMB**: Desplazamiento de la cámara
    - **ALT + LMB**: Orbitar el objeto seleccionado
    - **F**: Centra la cámara en el objeto seleccionado
