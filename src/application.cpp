#include "application.h"

using namespace engix;

engix::Application::Application() 
: window("Pottery!", 1280, 720), logoTimer(000)
{
    currentPage = Page::LOGO;

    loadAssets();

    mouse = Mouse(textures["cursor"]);

    auto doNothing = [](VisualElement*, const Mouse&){};

    auto background = std::make_shared<VisualElement>(std::make_shared<Texture>(templates["background"]->genTexture(gScreen.width, gScreen.height)));
    background->scaling(Texture::Scaling::NONE);
    for (auto& page : pages)
    {
        page.push_back(background);
    }
    
    pages[Page::LOGO].push_back(std::make_shared<VisualElement>(textures["logo"], gScreen.width, gScreen.height));
    pages[Page::LOGO].back()->scaling(Texture::Scaling::STRETCH);
    
    pages[Page::SEARCH].push_back(std::make_shared<Button>(textures["searchButton"]));
    pages[Page::SEARCH].back()->relativePos({3, 2});
    pages[Page::SEARCH].back()->horizontalAlign(engix::Align::END);

    auto textField = std::make_shared<InputBox>(templates["textField"], textures["textCursor"]);
    textField->font(fonts["default"]);
    textField->horizontalAlign(Align::CENTER);
    textField->verticalAlign(Align::CENTER);
    textField->padding(2, 1, 2, 1);
    textField->textColor("3c1705");
    textField->selectedTextColor("983f0a");
    textField->selectionColor("fde7ba");

    pages[Page::SEARCH].push_back(std::move(textField));

    logoTimer.start();
}

engix::Application::~Application()
{
}

void engix::Application::loadAssets()
{
    fonts["default"] = std::make_shared<Font>(assets::FONTS_PATH + "pixix");

    textures["cursor"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "cursor.png");
    textures["logo"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "logo.png");
    textures["searchButton"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "searchButton.png");
    textures["verticalSliderPoint"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "verticalSliderPoint.png");
    textures["textCursor"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "textCursor.png");

    templates["background"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "backgroundTemplate");
    templates["bigTextField"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "bigTextFieldTemplate");
    templates["label"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "labelTemplate");
    templates["sliderLine"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "sliderLineTemplate");
    templates["textField"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "textFieldTemplate");
}

void engix::Application::loop()
{
    while (input.isRunning)
    {
        input.update();
        update();
        render();
    }
}

void engix::Application::update()
{
    delta.restart();

    mouse.enable(input.cursor.isShown);

    logoTimer.update();
    if (currentPage == Page::LOGO && !logoTimer.isRunning())
    {
        currentPage = Page::SEARCH;
    }

    for (const auto& page : pages[currentPage])
    {
        if (page->enable())
            page->update(input);
    }
    if (mouse.enable())
        mouse.update(input);
}

void engix::Application::render()
{
    window.fill({"8a4009"});

    for (const auto& page : pages[currentPage])
    {
        if (page->enable())
            page->render();
    }

    if (mouse.enable())
        mouse.render();
    window.update();
}