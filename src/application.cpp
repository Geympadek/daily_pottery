#include "application.h"

using namespace engix;

engix::Application::Application() 
: window("Pottery!", 1600, 900), logoTimer(0), currentInput(&prevInput)
{
    currentPage = Page::LOGO;

    loadAssets();

    mouse = Mouse(textures["cursor"]);

    auto background = std::make_shared<VisualElement>(std::make_shared<Texture>(templates["background"]->genTexture(gScreen.width, gScreen.height)));

    std::vector<std::shared_ptr<VisualElement>> navigation;
    std::unordered_map<Page, std::shared_ptr<VisualElement>> currentPageNav;

    navigation.push_back(std::make_shared<VisualElement>(textures["temperButton"]));
    navigation.back()->relativePos({2, 1});
    navigation.back()->onClick([this](VisualElement*, Input&){
        this->currentPage = Page::TEMPER;
    });

    currentPageNav[Page::TEMPER] = std::make_shared<VisualElement>(textures["temperButtonPressed"]);
    currentPageNav[Page::TEMPER]->relativePos({2, 1});

    navigation.push_back(std::make_shared<VisualElement>(textures["mainInfoButton"]));
    navigation.back()->relativePos({2, 19});
    navigation.back()->onClick([this](VisualElement*, Input&){
        this->currentPage = Page::MAIN_INFO;
    });

    currentPageNav[Page::MAIN_INFO] = std::make_shared<VisualElement>(textures["mainInfoButtonPressed"]);
    currentPageNav[Page::MAIN_INFO]->relativePos({2, 19});
    
    navigation.push_back(std::make_shared<VisualElement>(textures["thicknessButton"]));
    navigation.back()->relativePos({2, 37});
    navigation.back()->onClick([this](VisualElement*, Input&){
        this->currentPage = Page::THICKNESS;
    });
    
    currentPageNav[Page::THICKNESS] = std::make_shared<VisualElement>(textures["thicknessButtonPressed"]);
    currentPageNav[Page::THICKNESS]->relativePos({2, 37});

    navigation.push_back(std::make_shared<VisualElement>(textures["dryingButton"]));
    navigation.back()->relativePos({2, 55});
    navigation.back()->onClick([this](VisualElement*, Input&){
        this->currentPage = Page::DRYING;
    });

    currentPageNav[Page::DRYING] = std::make_shared<VisualElement>(textures["dryingButtonPressed"]);
    currentPageNav[Page::DRYING]->relativePos({2, 55});

    navigation.push_back(std::make_shared<VisualElement>(textures["fireingButton"]));
    navigation.back()->relativePos({2, 73});
    navigation.back()->onClick([this](VisualElement*, Input&){
        this->currentPage = Page::FIREING;
    });

    currentPageNav[Page::FIREING] = std::make_shared<VisualElement>(textures["fireingButtonPressed"]);
    currentPageNav[Page::FIREING]->relativePos({2, 73});

    navigation.push_back(std::make_shared<VisualElement>(textures["successButton"]));
    navigation.back()->relativePos({2, 91});
    navigation.back()->onClick([this](VisualElement*, Input&){
        this->currentPage = Page::SUCCESS;
    });
    currentPageNav[Page::SUCCESS] = std::make_shared<VisualElement>(textures["successButtonPressed"]);
    currentPageNav[Page::SUCCESS]->relativePos({2, 91});

    background->scaling(Texture::Scaling::NONE);

    for (size_t i = 0; i < pages.size(); i++)
    {
        auto& page = pages[i];
        page.push_back(background);
        
        for (auto& btn : navigation)
        {
            page.push_back(btn);
        }

        auto pressedBtn = currentPageNav[static_cast<Page>(i)];
        if (pressedBtn != nullptr)
            page.push_back(pressedBtn);
    }
    
    pages[Page::LOGO].push_back(std::make_shared<VisualElement>(textures["logo"], gScreen.width, gScreen.height));
    pages[Page::LOGO].back()->scaling(Texture::Scaling::STRETCH);
    
    pages[Page::SEARCH].push_back(std::make_shared<Button>(textures["searchButton"]));
    pages[Page::SEARCH].back()->relativePos({3, 2});
    pages[Page::SEARCH].back()->horizontalAlign(engix::Align::END);

    auto temperLabel = std::make_shared<TextBox>(templates["bigTextField"]);
    temperLabel->relativePos({22, 2});
    temperLabel->width(153);
    temperLabel->height(14);
    temperLabel->paddingLeft(3);
    temperLabel->paddingUp(3);
    temperLabel->font(fonts["default"]);
    temperLabel->text(u"Пропорции сырья в процентах");
    temperLabel->textColor("3c1705");
    pages[Page::TEMPER].push_back(temperLabel);

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

    textures["temperButton"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "temperButton.png");
    textures["mainInfoButton"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "mainInfoButton.png");
    textures["thicknessButton"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "thicknessButton.png");
    textures["dryingButton"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "dryingButton.png");
    textures["fireingButton"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "fireingButton.png");
    textures["successButton"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "successButton.png");

    textures["temperButtonPressed"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "temperButtonPressed.png");
    textures["mainInfoButtonPressed"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "mainInfoButtonPressed.png");
    textures["thicknessButtonPressed"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "thicknessButtonPressed.png");
    textures["dryingButtonPressed"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "dryingButtonPressed.png");
    textures["fireingButtonPressed"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "fireingButtonPressed.png");
    textures["successButtonPressed"] = std::make_shared<Texture>(assets::TEXTURES_PATH + "successButtonPressed.png");

    templates["background"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "backgroundTemplate");
    templates["bigTextField"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "bigTextFieldTemplate");
    templates["label"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "labelTemplate");
    templates["sliderLine"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "sliderLineTemplate");
    templates["textField"] = std::make_shared<BoxTemplate>(assets::BOX_TEMPLATE_PATH + "textFieldTemplate");
}

void engix::Application::loop()
{
    while (currentInput.isRunning)
    {
        currentInput.update();
        update();
        render();
    }
}

void engix::Application::update()
{
    delta.restart();

    mouse.enable(currentInput.cursor.isShown);

    logoTimer.update();
    if (currentPage == Page::LOGO && !logoTimer.isRunning())
    {
        currentPage = Page::SEARCH;
    }

    pageUpdate:
    for (const auto& element : pages[currentPage])
    {
        auto prevPage = currentPage;
        if (element->enable())
            element->update(currentInput);
        
        if (prevPage != currentPage)
        {
            goto pageUpdate;
        }
    }
    if (mouse.enable())
        mouse.update(currentInput);
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