#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    Este componente renderiza un efecto visual animado.
    Heredamos de juce::Timer para poder actualizar el dibujo constantemente.
*/
class MainComponent  : public juce::Component,
                       public juce::Timer // <--- CAMBIO: Ahora somos un Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // Esta función se llama cada vez que el Timer hace "tick"
    void timerCallback() override; // <--- CAMBIO: Declaración obligatoria para la animación

private:
    //==============================================================================
    // Esta variable guardará el progreso del tiempo para los cálculos matemáticos
    float faseDeAnimacion = 0.0f; // <--- CAMBIO: Estado de la animación

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
