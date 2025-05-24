# quoteszsh - Mensajes de Bienvenida Inteligentes para tu Terminal

`quoteszsh` es una utilidad en C++ que te da la bienvenida a tu terminal con un diseño ASCII aleatorio y una frase generada por IA, con múltiples niveles de fallback para asegurar que siempre tengas un mensaje.

## Características

*   Frases generadas por IA a través de OpenRouter (API externa) o Ollama (localmente).
*   Múltiples diseños ASCII seleccionados al azar.
*   Prompts variados para diferentes tipos de frases (motivacionales, sobre Linux, programación, etc.).
*   Sistema de fallback inteligente:
    1.  Intenta OpenRouter.
    2.  Si falla, intenta Ollama local.
    3.  Si ambos fallan, muestra una frase predefinida con `cowsay` (si está instalado).
    4.  Si `cowsay` falla, muestra una frase predefinida simple.
*   Integración sencilla con `.zshrc` (u otros archivos de inicio de shell).

## Requisitos de Compilación (para construir desde el código fuente)

Estas son las herramientas y librerías necesarias para compilar `quoteszsh` tú mismo:

1.  **Compilador de C++ Moderno:**
    *   Se requiere un compilador que soporte C++17 o superior (el código usa características de C++17).
    *   Ejemplos:
        *   **GCC (g++)**: Versión 7 o superior.
        *   **Clang**: Versión 5 o superior.
    *   Instalación (Debian/Ubuntu): `sudo apt install build-essential g++`

2.  **CMake:**
    *   Sistema de gestión de la compilación. Versión 3.15 o superior recomendada.
    *   Instalación (Debian/Ubuntu): `sudo apt install cmake`

3.  **Git:**
    *   Necesario para que CMake (`FetchContent`) descargue las dependencias (librerías `cpp-httplib` y `nlohmann/json`).
    *   Instalación (Debian/Ubuntu): `sudo apt install git`

4.  **Librerías de Desarrollo de OpenSSL (para soporte HTTPS):**
    *   `cpp-httplib` necesita OpenSSL para realizar peticiones HTTPS a OpenRouter.
    *   Instalación (Debian/Ubuntu): `sudo apt install libssl-dev`
    *   Instalación (Fedora/RHEL): `sudo dnf install openssl-devel`

5.  **pkg-config (opcional pero recomendado):**
    *   Ayuda a CMake a encontrar librerías como OpenSSL.
    *   Instalación (Debian/Ubuntu): `sudo apt install pkg-config`

*(Las librerías `cpp-httplib` y `nlohmann/json` se descargan automáticamente por CMake durante el proceso de compilación, por lo que no necesitan instalación manual previa).*

## Requisitos de Ejecución (para usar el programa una vez compilado)

1.  **Ejecutable `quoteszsh` Compilado:**
    *   Deberás haber compilado el programa o descargado un binario precompilado (si lo proporcionas).

2.  **(Opcional, para Fallback a OpenRouter) API Key de OpenRouter:**
    *   Si quieres usar OpenRouter como fuente primaria de frases, necesitas una API Key de [OpenRouter.ai](https://openrouter.ai/).
    *   Esta clave debe configurarse como una variable de entorno: `export OPENROUTER_API_KEY="tu_clave_aqui"` (generalmente en tu `~/.zshrc` o `~/.bashrc`).

3.  **(Opcional, para Fallback a Ollama Local) Ollama y un Modelo Descargado:**
    *   Si quieres usar Ollama local como fallback (o como fuente primaria si modificas el código):
        *   **Instalar Ollama:** Sigue las instrucciones en [ollama.ai](https://ollama.ai/).
        *   **Descargar un Modelo:** Necesitas tener al menos un modelo de lenguaje descargado. El programa intentará usar el modelo especificado en el código (actualmente configurado para `qwen3-4b-abliterated-nothink:latest` como fallback, pero puedes cambiarlo en el código fuente o pasarlo como argumento si modificas la función `main`).
            *   Ejemplo para descargar un modelo: `ollama pull qwen:latest` (o `ollama pull mistral:latest`, `ollama pull llama3:latest`, etc.)
        *   **Ollama Corriendo:** El servicio de Ollama debe estar ejecutándose en `http://localhost:11434` (configuración por defecto).

4.  **(Opcional, para Fallback a Cowsay) `cowsay` Instalado:**
    *   Si tanto OpenRouter como Ollama fallan, el programa intentará usar `cowsay` con una frase predefinida.
    *   Instalación (Debian/Ubuntu): `sudo apt install cowsay`
    *   Puedes personalizar el personaje de `cowsay` (ej. `-f tux`) en el código si lo deseas.

5.  **Shell Compatible (Zsh recomendado):**
    *   Las instrucciones de integración están optimizadas para Zsh (`.zshrc`), pero el ejecutable en sí debería funcionar en cualquier terminal Linux. La integración con otros shells (`.bashrc`, `.fishrc`) requeriría ajustes menores en el archivo de inicio del shell.

## Instrucciones de Compilación

1.  Clona este repositorio:
    ```bash
    git clone https://github.com/tu_usuario/quoteszsh.git
    cd quoteszsh
    ```
2.  Crea un directorio de compilación y entra en él:
    ```bash
    mkdir build
    cd build
    ```
3.  Ejecuta CMake para configurar el proyecto (desde el directorio `build`):
    ```bash
    cmake .. 
    ```
    *(Esto configurará un build de tipo Debug por defecto. Para un build de Release, usa `cmake -DCMAKE_BUILD_TYPE=Release ..`)*
4.  Compila el proyecto:
    ```bash
    make -j$(nproc)  # o simplemente 'make'
    ```
    El ejecutable `quoteszsh` se encontrará en el directorio `build`.

## Instalación (Copia del Ejecutable)

Una vez compilado, copia el ejecutable a un directorio en tu `PATH`. Se recomienda `~/.local/bin/`:

```bash
mkdir -p ~/.local/bin
cp build/quoteszsh ~/.local/bin/
```
Asegúrate de que `~/.local/bin` esté en tu `PATH`.

## Configuración del Shell (Ejemplo para `.zshrc`)

Añade lo siguiente a tu archivo `~/.zshrc`:

```zsh
# ~/.zshrc

# Asegurar que ~/.local/bin esté en el PATH (si lo usas)
if [[ ":$PATH:" != *":$HOME/.local/bin:"* ]]; then
  export PATH="$HOME/.local/bin:$PATH"
fi

# (OPCIONAL) Exportar tu API Key de OpenRouter
export OPENROUTER_API_KEY="tu_api_key_de_openrouter_aqui" 

# Ejecutar quoteszsh al iniciar la terminal
if command -v quoteszsh &> /dev/null; then
  echo 
  quoteszsh
  # echo # Línea en blanco opcional después
fi

# (OPCIONAL) Si quieres el mensaje de bienvenida adicional debajo:
# function show_extra_welcome_info() {
#   function show_extra_welcome_info() { } # Para que solo se ejecute una vez
#   local welcome_line="¡Bienvenido, $USER, a tu terminal Debian!"
#   # ... (resto de tu mensaje de bienvenida adicional) ...
#   echo "$welcome_line" # etc.
# }
# autoload -Uz add-zsh-hook
# add-zsh-hook precmd show_extra_welcome_info
```

Recarga tu configuración de shell (`source ~/.zshrc`) o abre una nueva terminal.

## Solución de Problemas

*   **Errores de CMake o Compilación:** Asegúrate de tener todas las "Requisitos de Compilación" instaladas.
*   **"Error_API_KEY"**: Verifica que la variable de entorno `OPENROUTER_API_KEY` esté correctamente exportada y que la clave sea válida.
*   **"Error_OLLAMA_CONNECTION"**: Asegúrate de que el servicio de Ollama esté corriendo (`systemctl status ollama` o similar) y que el modelo especificado esté descargado (`ollama list`).
*   **Faltan diseños ASCII / Placeholder no encontrado**: Verifica la variable `asciiArts` y el string `placeholder` en `main.cpp`.

---
