#include <iostream>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Textbox.h"
#include "Binary.h"

const float button_Width = 80.f;
const float button_Height = 45.f;
const float node_Radius = 22.f;








// Calculates the text position to place
template <typename pos_Shape, typename pos_Text>
sf::Vector2f text_Calc(pos_Shape shape_Value, pos_Text text_Value)
{
    return sf::Vector2f(-(shape_Value.getOrigin().x) + shape_Value.getGlobalBounds().width / 2 - text_Value.getGlobalBounds().width / 2 - 3, 
        -(shape_Value.getOrigin().y) + shape_Value.getGlobalBounds().height / 2 - 12);
}

// Calculates the arm position to place
sf::Vector2f arm_Calc(sf::CircleShape shape_Value)
{
    return sf::Vector2f(-(shape_Value.getOrigin().x) + shape_Value.getGlobalBounds().width / 2, 
        -(shape_Value.getOrigin().y) + shape_Value.getGlobalBounds().height / 2);
}

// Calculates the node position to place
sf::Vector2f node_Calc(sf::Vector3f width, sf::CircleShape shape_Value)
{
    return sf::Vector2f(-(width.x - shape_Value.getGlobalBounds().width / 2), -width.y);
}




// Calculates the arm angle
float arm_Angle(float denominator)
{
    if(denominator < 0)
        return atan(100.f / denominator) * 180.f / 3.1415f;

    return -(180 - (atan(100.f / denominator) * 180.f / 3.1415f));
}

// Calculates the arm length
float arm_Length(float denominator)
{
    return sqrt(pow(100, 2) + pow(denominator, 2));
}









int main()
{
    // Binary Search Tree object declaration
    BST b, *root = nullptr;

    // Sets the level of anti-aliasing (Hardware and OS dependent)
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1200, 680), "Binary Tree", sf::Style::Titlebar | sf::Style::Default);
    sf::Vector2i centerWindow((sf::VideoMode::getDesktopMode().width / 2) - 640, (sf::VideoMode::getDesktopMode().height / 2) - 400);
	window.setPosition(centerWindow);

    // Disables the key repeating feature
	window.setKeyRepeatEnabled(false);

    // Calculates the width of the display window
    float window_Size_Width = window.getSize().x;

    // Vector of circle, text and nodeArm for Insert process
    std::vector<sf::CircleShape> nodes;
    std::vector<sf::Text> contents;
    std::vector<sf::RectangleShape> lines;

    // Vector of circle, nodeArm for Search process (only instant)
    std::vector<sf::CircleShape> highlight_Node;
    std::vector<sf::RectangleShape> highlight_Line;

    // Sets the maximum fps of screen
    window.setFramerateLimit(60);

    // Loads the font 
    sf::Font font;
    if (!font.loadFromFile("./font/arial.ttf"))    // Check for error condition
        return EXIT_FAILURE;
    
    // Loads the top left icon on window
    sf::Image icon;
    if(!icon.loadFromFile("./images/tree.jpg"))    // Check for error condition
        return EXIT_FAILURE;
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());    // Sets the top left icon on the window

    // Loads the texture of buttons
    sf::Texture buttons;
    if(!buttons.loadFromFile("./images/wood-texture.jpg"))    // Checks the error condition
        return EXIT_FAILURE;

    // Loads the texture for clicked buttons
    sf::Texture clickButton;
    if(!clickButton.loadFromFile("./images/wood-texture-clicked.jpg"))    // Checks the error condition
        return EXIT_FAILURE;
    

    // Insert button
    sf::RectangleShape insert(sf::Vector2f(button_Width, button_Height));    // Creates rectangle for the button
    insert.setTexture(&buttons);    // Sets the texture of the button
    insert.setTextureRect(sf::IntRect(10, 10, 50, 50)); // Loads the portion of texture starting from point (10, 10) with area of 50 X 50
    insert.setOrigin(-10.f,-10.f);  // Sets the origin of the button
    insert.setOutlineThickness(1.f);    // Sets the outline thickness of the button
    insert.setOutlineColor(sf::Color::Blue);    // Sets the color of the outline of the button

    // Delete button
    sf::RectangleShape deletes(sf::Vector2f(button_Width, button_Height));    // Creates rectangle for the button
    deletes.setTexture(&buttons);    // Sets the texture of the button
    deletes.setTextureRect(sf::IntRect(10, 10, 50, 50));    // Loads the portion as above
    deletes.setOrigin(sf::Vector2f(-(-(insert.getOrigin().x) + insert.getGlobalBounds().width + 20), -10.f));    // Sets the origin of the button
    deletes.setOutlineThickness(1.f);    // Sets the ouline thickness of the button
    deletes.setOutlineColor(sf::Color::Blue);    // Sets the color of the outline of the button

    // Search button
    sf::RectangleShape search(sf::Vector2f(button_Width, button_Height));    // Creates rectangle for the button
    search.setTexture(&buttons);    // Sets the texture of the button
    search.setTextureRect(sf::IntRect(10, 10, 50, 50));    // Loads the portion as above  
    search.setOrigin(sf::Vector2f(-(-(deletes.getOrigin().x) + deletes.getGlobalBounds().width + 20), -10.f));    // Sets the origin of the button
    search.setOutlineThickness(1.f);    // Sets the outline thickness of the button
    search.setOutlineColor(sf::Color::Blue);    // Sets the color of the outline of the button

    // Create a graphical text to display
    sf::Text text_Insert("Insert", font, 14.f);    // Inputs the text to be displaayed, font family and font size
    text_Insert.setFillColor(sf::Color::Blue);    // Sets the color of the font
    text_Insert.setPosition(text_Calc<sf::RectangleShape, sf::Text>(insert, text_Insert));    // Sets the position of the text

    sf::Text text_Delete("Delete", font, 14.f);    // Same as above
    text_Delete.setFillColor(sf::Color::Blue);    // Same as above
    text_Delete.setPosition(text_Calc<sf::RectangleShape, sf::Text>(deletes, text_Delete));    // Same as above

    sf::Text text_Search("Search", font, 14.f);    // Same as above
    text_Search.setFillColor(sf::Color::Blue);    // Same as above
    text_Search.setPosition(text_Calc<sf::RectangleShape, sf::Text>(search, text_Search));    // Same as above

    // Textbox
    Textbox insert_Text(25, sf::Color::White, true);    // Creates a textbox area
    insert_Text.setPosition(sf::Vector2f(-(search.getOrigin().x) + search.getGlobalBounds().width + 15, 10.f));    // Sets the position of the textbox
    insert_Text.setLimit(true, 3);    // Sets the limit true and of 3
    insert_Text.setFont(font);    // Sets the font for the text
    
    int a = 0;

    // Sound when clicking buttons
    sf::SoundBuffer click;
    if(!click.loadFromFile("./audio/click.ogg"))    // Checks the error condition
        return EXIT_FAILURE;
    sf::Sound mouse_Click(click);    // Sets the clicking sound

    // Start the game loop
    while (window.isOpen())
    {
        std::string convert;

        // Process events
        sf::Event event;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            insert_Text.setSelected(true);

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            insert_Text.setSelected(false);

        while (window.pollEvent(event))
        {
            int node_Content = 0;
            convert = insert_Text.getText();
            std::stringstream testing(convert);
            testing >> node_Content;

            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();    
            
            if (event.type == sf::Event::TextEntered)
            {
                insert_Text.typedOn(event);
            }

            if(event.type = sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left && insert.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) && a % 2 == 0)
                {
                    insert.setTexture(&clickButton);
                    insert.setTextureRect(sf::IntRect(10, 10, 50, 50));
                    insert.setOutlineColor(sf::Color::Green);
                    insert.setOutlineThickness(1.7f);
                    text_Insert.setFillColor(sf::Color::Green);
                    a++;
                    mouse_Click.play();
   
                    if(node_Content > 0)
                    {
                        if (root == nullptr)
                            root = b.Insert(root, node_Content);

                        else    
                            b.Insert(root, node_Content);

                        if(!b.duplicate(node_Content))
                        {
                            sf::Vector3f node_Position = b.node_Position(node_Content, window_Size_Width);

                            float denominator = node_Position.x - node_Position.z;

                            sf::CircleShape nodes_Circle(node_Radius);
                            sf::Text node_Text(convert, font, 20.f);

                            float length = arm_Length(denominator);

                            if(abs(denominator) < 1)
                                length = 0;

                            sf::RectangleShape node_Line(sf::Vector2f(length, 0.f));

                            nodes_Circle.setFillColor(sf::Color::White);
                            nodes_Circle.setOutlineThickness(2.5f);
                            nodes_Circle.setOutlineColor(sf::Color::Black);
                            nodes_Circle.setOrigin(node_Calc(node_Position, nodes_Circle));
                            
                            node_Text.setFillColor(sf::Color::Black);
                            node_Text.setPosition(text_Calc<sf::CircleShape, sf::Text>(nodes_Circle, node_Text));

                            node_Line.setOutlineThickness(2.2f);
                            node_Line.setOutlineColor(sf::Color::Blue);
                            node_Line.setRotation(arm_Angle(denominator));
                            node_Line.setPosition(arm_Calc(nodes_Circle));

                            nodes.push_back(nodes_Circle);
                            contents.push_back(node_Text);
                            lines.push_back(node_Line);
                            highlight_Node.clear();
                            highlight_Line.clear();
                        }
                    }
                }

                else if(event.mouseButton.button == sf::Mouse::Left && insert.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) && a % 2 != 0)
                {
                    insert.setTexture(&buttons);
                    insert.setTextureRect(sf::IntRect(10, 10, 50, 50));
                    insert.setOutlineThickness(1.f);
                    insert.setOutlineColor(sf::Color::Blue);
                    text_Insert.setFillColor(sf::Color::Blue);
                    a++;
                }

                else if (event.mouseButton.button == sf::Mouse::Left && deletes.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) && a % 2 == 0)
                {
                    deletes.setTexture(&clickButton);
                    deletes.setTextureRect(sf::IntRect(10, 10, 50, 50));
                    deletes.setOutlineColor(sf::Color::Green);
                    deletes.setOutlineThickness(1.7f);
                    text_Delete.setFillColor(sf::Color::Green);
                    a++;
                    mouse_Click.play();

                    b.Delete(root, node_Content);
                }

                else if(event.mouseButton.button == sf::Mouse::Left && deletes.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) && a % 2 != 0)
                {
                    deletes.setTexture(&buttons);
                    deletes.setTextureRect(sf::IntRect(10, 10, 50, 50));
                    deletes.setOutlineThickness(1.f);
                    deletes.setOutlineColor(sf::Color::Blue);
                    text_Delete.setFillColor(sf::Color::Blue);
                    a++;
                }

                else if (event.mouseButton.button == sf::Mouse::Left && search.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) && a % 2 == 0)
                {
                    search.setTexture(&clickButton);
                    search.setTextureRect(sf::IntRect(10, 10, 50, 50));
                    search.setOutlineColor(sf::Color::Green);
                    search.setOutlineThickness(1.7f);
                    text_Search.setFillColor(sf::Color::Green);
                    a++;
                    mouse_Click.play();

                    // Clears node before next search
                    highlight_Node.clear();
                    highlight_Line.clear();

                    if(b.Search(root, node_Content) != nullptr)
                    {
                        sf::Vector3f node_Position_Search = b.node_Position(node_Content, window_Size_Width);

                        float denominator = node_Position_Search.x - node_Position_Search.z;
                        
                        sf::CircleShape highlight_Circle(node_Radius);
                        float length = arm_Length(denominator);

                        if(abs(denominator) < 1)
                            length = 0;

                        sf::RectangleShape highlight_Arm(sf::Vector2f(length, 0.f));

                        highlight_Circle.setOutlineThickness(3.5f);
                        highlight_Circle.setOutlineColor(sf::Color::Red);
                        highlight_Circle.setOrigin(node_Calc(node_Position_Search, highlight_Circle));
                        highlight_Circle.setFillColor(sf::Color::Transparent);

                        highlight_Arm.setOutlineThickness(3.f);
                        highlight_Arm.setOutlineColor(sf::Color::Red);
                        highlight_Arm.setRotation(arm_Angle(denominator));
                        highlight_Arm.setPosition(arm_Calc(highlight_Circle));

                        highlight_Node.push_back(highlight_Circle);
                        highlight_Line.push_back(highlight_Arm);
                    }
                }

                else if(event.mouseButton.button == sf::Mouse::Left && search.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))) && a % 2 != 0)
                {
                    search.setTexture(&buttons);
                    search.setTextureRect(sf::IntRect(10, 10, 50, 50));
                    search.setOutlineThickness(1.f);
                    search.setOutlineColor(sf::Color::Blue);
                    text_Search.setFillColor(sf::Color::Blue);
                    a++;
                }
            }
        }

        // Clear screen
        window.clear(sf::Color(85, 75, 49));

        // Displays the textbox content
        insert_Text.drawTo(window);

        // Draw the buttons
        window.draw(insert);
        window.draw(deletes);
        window.draw(search);

        // Draw the string
        window.draw(text_Insert);
        window.draw(text_Delete);
        window.draw(text_Search);

        // Draws the node lines
        for(auto &l : lines)
            window.draw(l);

        for(auto &ha : highlight_Line)
            window.draw(ha);

        // Draws the actual nodes
        for(auto &r: nodes)
            window.draw(r);

        // Draws the highlight circle
        for(auto &h : highlight_Node)
            window.draw(h);
        
        // Draws the node contents
        for(auto &t : contents)
            window.draw(t);

        // Updates the window
        window.display();
    }
    return EXIT_SUCCESS;
}