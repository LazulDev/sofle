# Keymaps

Al modificar una capa de un keymap, alinea con espacios los bindings de todas sus filas para que cada posición física del teclado se lea en la misma columna. Si un binding necesita más espacio, ensancha esa columna en toda la capa.

## Diagrama del README

Cuando cambies un binding en `config/eyelash_sofle.keymap`, comprueba que su posición en el diagrama muestra el carácter que escribe o la función que ejecuta.

- Para teclas de escritura, configura las leyendas en `parse_config.zmk_keycode_map` de `keymap_drawer.config.yaml`. Usa los caracteres de Español ISO de macOS. Sigue la convención de cada capa: en `Write`, los números, la coma y el punto usan `tap` para el carácter principal, `shifted` para Shift y `hold` para la leyenda inferior de Opción. En `Symbols`, muestra solo el símbolo principal. Conserva la guarda `KEYMAP_DRAWER` que permite al generador leer los nombres `ES_*`.
- Para ajustes del teclado, como iluminación o Bluetooth, prioriza un icono descriptivo. Elige uno de [Material Design Icons](https://pictogrammers.com/library/mdi/) y asígnalo en `parse_config.raw_binding_map` al binding completo. Por ejemplo, `"&rgb_ug RGB_TOG": '$$mdi:lightbulb-outline$$'`. Si cambian los parámetros del binding, actualiza también la clave de la regla.
- Haz la corrección en la configuración del generador para que sobreviva a la regeneración. El workflow **Draw Keymap** se ejecuta al subir cambios en `config/**` o en `keymap_drawer.config.yaml` y también se puede lanzar manualmente. Comprueba la leyenda en `keymap-drawer/eyelash_sofle.yaml` y abre `keymap-drawer/eyelash_sofle.svg` para verificar que se lee en la tecla y capa correctas.
