# Estacion Espacial v4 - Modular Structure
# 🚀 Estación Espacial

Bienvenido a **Estación Espacial**, un juego 2D por turnos donde deberás abrirte camino a través de una estación llena de obstáculos para alcanzar la salida. Utiliza tu ingenio, administra tu energía y batería, y prepárate para interactuar con puertas, tanques de atmósfera y muros destructibles.

---

## 🧹 Descripción general

En este juego serás un astronauta atrapado en una estación espacial con sistemas dañados. Tu misión es llegar a la celda de salida (`F`) usando tus habilidades limitadas. A medida que avanzas, deberás:

* **Recolectar atmósferas** (`A` y `B`) que permiten abrir puertas (`D`)
* **Administrar batería**, que se consume con cada paso
* **Cargar energía** y usarla para romper paredes cuando no haya otra salida

Hay tres niveles de dificultad y un modo automático donde un bot resuelve el mapa paso a paso.

---

## 👷️ Instrucciones de compilación

### Requisitos

* C++17 o superior
* [SFML 2.5+](https://www.sfml-dev.org/download.php)
* CMake (opcional, pero recomendado)

### Compilación manual (sin CMake)

1. Asegúrate de tener SFML correctamente instalado.
2. Usa un compilador compatible. Ejemplo con `g++`:

```bash
g++ -std=c++17 -I include/ src/*.cpp -o EstacionEspacial -lsfml-graphics -lsfml-window -lsfml-system
```

> Asegúrate de que la ruta a las librerías y la fuente (`.ttf`) esté correctamente configurada.

### Compilación con CMake (recomendado)

1. Crea una carpeta de build:

```bash
mkdir build && cd build
cmake ..
make
```

2. Ejecuta el juego:

```bash
./EstacionEspacial
```

---

## 🎮 Cómo jugar

1. Al iniciar el juego, verás un **menú con 6 opciones**:

   * Jugar Fácil
   * Jugar Medio
   * Jugar Difícil
   * Bot Demo Fácil
   * Bot Demo Medio
   * Bot Demo Difícil
   * Salir

2. Usa las **flechas arriba/abajo** para seleccionar y presiona **Enter**.

3. En el modo de juego:

   * Explora el mapa
   * Recolecta atmósferas para abrir puertas
   * Rompe muros si tienes energía suficiente
   * Llega a la meta `F` antes de que se acabe la batería

4. Si lograslo, verás una pantalla de victoria. Si la batería llega a 0 antes, perderás.

---

## 🎯 Controles

| Tecla              | Acción                                               |
| ------------------ | ---------------------------------------------------- |
| `W`, `A`, `S`, `D` | Mover al jugador (arriba, izquierda, abajo, derecha) |
| `E`                | Romper un muro (si tienes energía máxima)            |
| `ESC`              | Volver al menú principal                             |
| `Enter`            | Confirmar selección en el menú                       |
| `↑`, `↓`           | Navegar por el menú                                  |

---

¡Buena suerte, astronauta! 🌌
