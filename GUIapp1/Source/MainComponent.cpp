#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Tamaño inicial de la ventana
    setSize (800, 600);

    // Arrancamos el temporizador a 60 cuadros por segundo (60Hz) para una animación suave.
    startTimerHz (60); // <--- CAMBIO: Iniciar animación
}

MainComponent::~MainComponent()
{
    // Es buena práctica detener el timer en el destructor
    stopTimer();
}

//==============================================================================
void timerCallback()
{
    // Esta función está vacía aquí, pero debe estar definida en el .cpp
}

// Necesitamos definir la función que declaramos en el .h fuera de la clase si la pusimos ahí,
// o simplemente implementarla. El estándar es implementarla así:

void MainComponent::timerCallback()
{
    // Incrementamos la fase. Un valor pequeño para un movimiento lento y suave.
    faseDeAnimacion += 0.015f;

    // Le decimos a JUCE que el componente necesita redibujarse.
    // Esto llamará a paint() en el siguiente ciclo de la GUI.
    repaint(); // <--- CAMBIO: Crucial para que haya movimiento
}


//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Aseguramos que getLocalBounds() nos de floats para cálculos precisos)
    auto area = getLocalBounds().toFloat();
    auto centro = area.getCentre();

    // 1. REQUISITO: Fondo Morado base
    g.fillAll (juce::Colours::purple);

    // Campo Cósmico Animado

    // Vamos a dibujar múltiples "capas" o líneas matemáticas entrelazadas
    const int numeroDeLineas = 25;

    for (int i = 0; i < numeroDeLineas; ++i)
    {
        // Calculamos un índice normalizado (de 0.0 a 1.0) para esta línea
        float indexNorm = (float)i / (float)numeroDeLineas;

        // --- CÁLCULO DE COLOR DINÁMICO ---
        // Usamos el índice de la línea y la fase de animación para cambiar el color.
        // Esto crea un degradado que fluye.
        float hue = indexNorm + (faseDeAnimacion * 0.1f);
        // Nos aseguramos de que el Hue esté entre 0.0 y 1.0
        while (hue > 1.0f) hue -= 1.0f;

        // Creamos un color vibrante (HSL) y le damos opacidad (Alpha) baja
        // para que las líneas se superpongan y brillen.
        auto colorLinea = juce::Colour::fromHSV (hue, 0.8f, 1.0f, 0.4f);
        g.setColour (colorLinea);

        // --- CÁLCULO DE GEOMETRÍA (Matemáticas de ondas) ---
        juce::Path wavePath;

        // Usamos fórmulas que combinan el índice de la línea con funciones Seno (sin)
        float radioBase = 100.0f + indexNorm * 150.0f;
        float distorsion = 40.0f * std::sin (faseDeAnimacion + indexNorm * juce::MathConstants<float>::twoPi * 2.0f);
        float radioFinal = radioBase + distorsion;

        // Dibujamos una forma elíptica distorsionada por las ondas
        wavePath.addCentredArc (centro.x, centro.y,
                                radioFinal, radioFinal * 0.6f, // Ancho y alto diferentes
                                faseDeAnimacion * (i % 2 == 0 ? 1.0f : -1.0f), // Rotación animada, alterna dirección
                                0.0f, juce::MathConstants<float>::twoPi, // Arco completo (círculo)
                                true);

        // Grosor de línea variable
        g.strokePath (wavePath, juce::PathStrokeType (1.0f + indexNorm * 3.0f));
    }

    // Opcional: Agregar un ligero resplandor central (Glow)
    juce::ColourGradient glowGradient (juce::Colours::white.withAlpha (0.2f), centro,
                                       juce::Colours::transparentBlack, centro.withX(area.getWidth()),
                                       true);
    g.setGradientFill (glowGradient);
    g.fillEllipse (centro.x - 200, centro.y - 200, 400, 400);


    // 3. Texto "Learning JUCE" (Mejorado para el efecto)

    // Fondo oscuro semi-transparente detrás del texto para que sea legible
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.fillRoundedRectangle (area.withSizeKeepingCentre (400, 80), 20.0f);

    // Texto blanco, grande y centrado
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (60.0f).withStyle ("Bold"));    g.drawText ("Learning JUCE",
                getLocalBounds(),
                juce::Justification::centred,
                true);
}

void MainComponent::resized()
{
    // En este caso, no necesitamos mover componentes hijos,
    // ya que todo se dibuja relativo al centro en paint().
}
