# Vivarium

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>
#include <iostream>

//configurable variables
float lampPowerIncreaseSpeed = 0.015;
float stormVolumeIncreaseSpeed = 0.015;
float stormVolumeDecreaseSpeed = 0.005;
float volumePickUpForFlash = 70.0;
float stormDuration = 5.0;
float tempHighCutoff = 24;
float tempLowCutoff = 20;
float canopyFanSpeedMax = 80;
float canopyFanSpeedMin = 20;
float canopyFanSpeedUp = 0.015;

//non-configurable variable
sf::Clock timerClock;
float lampPower = 0.0;
float humidity = 70.0;
bool mistKingOn = false;
sf::Font mainFont;
sf::Text lampPowerText;
sf::Text humidityText;
sf::Text volumeText;
sf::Text temperatureText;
sf::Text fanSpeedText;
bool stormGoing = false;
float stormVolume;
sf::Clock stormStarted;
float temperature;
float canopyFanSpeed = 20.0;

void mistKingRelay(bool onOrOff)
{
	if (onOrOff == true)
	{
		if (mistKingOn == false)
		{
			//turn on mistKing relay
			mistKingOn = true;
		}	
	}
	else
	{
		if (mistKingOn == true)
		{
			//turn off mistKing relay
			mistKingOn = false;
		}
	}
	
}

float checkHumidity()
{
	//return normalized value 0 - 100 from sensor
	return 60.0f;
}

void changeLampPower(float power)
{
	lampPower = power;
}

void t5Relay(bool onOrOff)
{
	if (onOrOff == true)
	{
		//turn on t5 relay
	}
	else
	{
		//turn off t5 relay
	}
}

void changeVolume(float vol)
{
	stormVolume = vol;
}

float CheckVolume()
{
	//detect real volume from mic and return normalized value 0 - 100
	return 75.00;
}

void Flash(bool onOrOff)
{
	if (onOrOff == true){
		//set LED pin to 1
	}
	else
	{
		//set LED pin to 0
	}
	
}

float checkTemperature()
{
	//return normalized decimal value from temperature sensor
	return 22.0;
}

//----------------------------------------------------------------------------------------------------------------//


void Storm(float dt){

	/*
	turn off relay t5
	ramp down lights to 20%
	ramp up sound
	start misting relay and single light flashing (in time with sound)
	wait 45 seconds
	ramp down sounds (over 20 seconds)
	turn off sounds and misting
	ramp up lights to 100%
	turn on relay t5
	*/

	t5Relay(false);

		//ramp down lamp
		if (lampPower - (lampPowerIncreaseSpeed * dt) > 20.0)
		{
			if (stormStarted.getElapsedTime().asSeconds() < stormDuration)
			{
				//decrease light power using time since last tick for smoothness
				changeLampPower(lampPower - (lampPowerIncreaseSpeed * dt));
				stormStarted.restart();
				//stop the storm function here.
				return;
			}
		}
		else
		{
			if (stormStarted.getElapsedTime().asSeconds() < stormDuration)
			{
				//lamp power has reached 20%
				changeLampPower(20.0);
			}
		}

			//check if sound is playing above the set value of volumePickUpForFlash and switch LED on if so
			if (CheckVolume() > volumePickUpForFlash)
			{
				Flash(true);
			}
			else
			{
				Flash(false);
			}

				if (stormVolume + (stormVolumeIncreaseSpeed * dt) < 100.0)
				{
					if (stormStarted.getElapsedTime().asSeconds() < stormDuration)
					{
						//increase volume using time since last tick for smoothness
						changeVolume(stormVolume + (stormVolumeIncreaseSpeed * dt));
						stormStarted.restart();
						//stop the storm function here
						return;
					}
				}
				else
				{
					if (stormStarted.getElapsedTime().asSeconds() < stormDuration)
					{
						//volume has reached 100%
						changeVolume(100.0);
					}
				}

					mistKingRelay(true);

						//end function here if 45 seconds hasnt elapsed
					if (stormStarted.getElapsedTime().asSeconds() < stormDuration)
						{
							return;
						}

					if (stormVolume - (stormVolumeDecreaseSpeed * dt) > 0.0)
					{
						//decrease volume using time since last tick for smoothness
						changeVolume(stormVolume - (stormVolumeDecreaseSpeed * dt));
						//stop the storm function here
						return;
					}
					else
					{
						//volume has reached 0%
						changeVolume(0.0);
					}

				mistKingRelay(false);

			//ramp up lamp
			if (lampPower + (lampPowerIncreaseSpeed * dt) < 100.0)
			{
				//increase light power using time since last tick for smoothness
				changeLampPower(lampPower + (lampPowerIncreaseSpeed * dt));
				//stop the storm function here.
				return;
			}
			else
			{
				//lamp power has reached 100%
				changeLampPower(100.0);
			}

		t5Relay(true);

}


void setText(sf::Text* text, sf::Font* font, sf::Vector2f pos, int size, sf::Color colour)
{

	text->setFont(*font);
	text->setPosition(pos);
	text->setColor(colour);
	text->setCharacterSize(size);

}


void render(sf::RenderWindow* window)
{

	
	lampPowerText.setString( "Lamp Power: " + std::to_string(lampPower) );
	humidityText.setString("Humidity: " + std::to_string(humidity));
	volumeText.setString("Volume: " + std::to_string(stormVolume));
	temperatureText.setString("Temperature: " + std::to_string(temperature) + " Degrees");
	fanSpeedText.setString("Fan Speed: " + std::to_string(canopyFanSpeed));

	window->clear();

	window->draw(lampPowerText);
	window->draw(humidityText);
	window->draw(volumeText);
	window->draw(temperatureText);
	window->draw(fanSpeedText);

	window->display();

}


void MainLoop(sf::RenderWindow* window)
{
	//loop until program closed
	while (window->isOpen())
	{
		//get time since last tick, in milliseconds (aiming for 0.16, for 60fps)
		float dt = timerClock.restart().asMilliseconds();

		//get input events
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::S)
				{
					stormGoing = true;
					stormStarted.restart();
				}
			}
		}

		//temperature monitoring
		temperature = checkTemperature();
		if (temperature > tempHighCutoff)
		{
			//ramp up fan
			if (canopyFanSpeed + (canopyFanSpeedUp * dt) < canopyFanSpeedMax)
			{
				canopyFanSpeed += (canopyFanSpeedUp * dt);
			}
			else
			{
				canopyFanSpeed = canopyFanSpeedMax;
			}
		}
		else if (temperature < tempLowCutoff)
		{
			//ramp down fan
			if (canopyFanSpeed - (canopyFanSpeedUp * dt) > canopyFanSpeedMin)
			{
				canopyFanSpeed -= (canopyFanSpeedUp * dt);
			}
			else
			{
				canopyFanSpeed = canopyFanSpeedMin;
			}
		}

		//get time of day
		time_t now = time(0);
		tm *ltm = localtime(&now);
		int hour = ltm->tm_hour;


		//storm
		if (stormGoing == true)
		{
			Storm(dt);
		}
		//check time of day
		if (hour >= 9 && hour < 10)
		{
			//ramp up lamp
			if (lampPower + (lampPowerIncreaseSpeed * dt) < 100.0)
			{
				//increase light power using time since last tick for smoothness
				changeLampPower(lampPower + (lampPowerIncreaseSpeed * dt));
			}
			else
			{
				//lamp power has reached 100%
				changeLampPower(100.0);
				t5Relay(true);
			}
		}
		else if (hour >= 10 && hour < 20)
		{
			humidity = checkHumidity();
			if (humidity < 70.0)
			{
				mistKingRelay(true);
			}
			else
			{
				mistKingRelay(false);
			}
		}
		else if (hour >= 20 && hour < 21)
		{
			//turn off t5 relay
			if (lampPower == 100.0)
			{
				t5Relay(false);
			}
			//ramp down lamp
			if (lampPower - (lampPowerIncreaseSpeed * dt) > 0.0)
			{
				//decrease light power using time since last tick for smoothness
				changeLampPower(lampPower - (lampPowerIncreaseSpeed * dt));
			}
			else
			{
				//lamp power has reached 0%
				changeLampPower(0.0);
			}
		}

		//render to the display device
		render(window);
	
	}

}


int main()
{

	//create window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Pi software");
	window.setFramerateLimit(60);

	//load font file for readouts
	if (!mainFont.loadFromFile("Media/Fonts/arial.ttf"))
	{
		std::cout << "Error loading font" << std::endl;
	}

	//assign font, position, colour etc to text variables
	setText(&lampPowerText, &mainFont, sf::Vector2f(10, 10), 20, sf::Color::White);
	setText(&humidityText, &mainFont, sf::Vector2f(10, 40), 20, sf::Color::White);
	setText(&volumeText, &mainFont, sf::Vector2f(10, 70), 20, sf::Color::White);
	setText(&temperatureText, &mainFont, sf::Vector2f(10, 100), 20, sf::Color::White);
	setText(&fanSpeedText, &mainFont, sf::Vector2f(10, 130), 20, sf::Color::White);

	//get time to set lights correctly on startup
	time_t now = time(0);
	tm *ltm = localtime(&now);
	int hour = ltm->tm_hour;
	if (hour > 9 && hour < 21)
	{
		t5Relay(true);
		lampPower = 100.0;
	}
	else
	{
		t5Relay(false);
		lampPower = 0.0;
	}

	//run mainLoop function forever (sends a pointer(&) of the window to the function)
	MainLoop(&window);

	//return out of program after mainLoop stops (this is where memory cleanup would be done if needed)
	return 0;
}
