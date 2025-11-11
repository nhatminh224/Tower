#include "InformationState.h"
#include "Utility.h"
#include "Foreach.h"
#include "ResourceHolder.h"
#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

static sf::FloatRect makeLetterboxViewport(sf::Vector2u winSize)
{
	const float viewW = 1920.f, viewH = 1080.f;
	float windowRatio = static_cast<float>(winSize.x) / winSize.y;
	float viewRatio = viewW / viewH;

	float sizeX = 1.f, sizeY = 1.f, posX = 0.f, posY = 0.f;
	if (windowRatio > viewRatio) {
		sizeX = viewRatio / windowRatio;
		posX = (1.f - sizeX) * 0.5f;
	}
	else {
		sizeY = windowRatio / viewRatio;
		posY = (1.f - sizeY) * 0.5f;
	}
	return { posX, posY, sizeX, sizeY };
}


InformationState::InformationState(StateStack& stack, Context context)
	: State(stack, context)
{
	sf::Texture& texture = context.textures->get(Textures::infoPanel);
	mBackgroundSprite.setTexture(texture);
	mBackgroundSprite.setPosition(0.f, 0.f);

	// UI view: 1920x1080 logical canvas
	mUiView = sf::View({ 0.f, 0.f, 1920.f, 1080.f });
	mUiViewport = makeLetterboxViewport(context.window->getSize());
	mUiView.setViewport(mUiViewport);

	// Close Info Button
	mCloseInfoButton.setTexture(context.textures->get(Textures::closeButton));
	mCloseInfoButton.setPosition(1400.f, 290.f);
	centerOrigin(mCloseInfoButton);

	// Font
	mFont = context.fonts->get(Fonts::KnightWarrior);

	// These coordinates should match the box in your background image
	const sf::Vector2f textAreaPosition(640.f, 300.f);  // Top-left of visible box
	const sf::Vector2f textAreaSize(770.f, 440.f);     // Dimensions of visible box

	// Create text elements
	std::vector<std::string> lines = {
		"CỐT TRUYỆN:",
"Nhà máy đang bị chính những tạo vật của nó bao vây. Một AI độc hại đã biến thành",
"robot phục vụ trở thành những kẻ thực thi pháp luật chết người. Thứ được tạo ra để phục vụ giờ đây lại đang tìm kiếm",
"thống trị".,
"",
"Bạn phải chiến đấu qua các khu vực do máy móc kiểm soát và tiêu diệt kẻ thù",
"trước khi chúng mở rộng ảnh hưởng ra ngoài tường thành. Thất bại không phải là một lựa chọn.",
"Cứu nhà máy. Mọi thứ đều phụ thuộc vào bạn.",
"Chúc may mắn, chiến binh cao quý của chúng ta.",
"",
"ĐỘI",
"Một trận đấu của",
"ĐỘI 7 up",

"",
"LOẠI KẺ THÙ:",
"Trinh sát nhanh:",
"Đơn vị nhanh nhất trên chiến trường. Những con robot hạng nhẹ này rất tuyệt vời",
"cho chiến thuật đánh nhanh rút gọn, nhưng lại rất mỏng manh.",
"",
"Cơ giáp tầm xa:",
"Một đơn vị có tốc độ và máu cân bằng. Chúng duy trì tốc độ khá nhanh",
"mà không quá dễ bị tiêu diệt".,
"",
"Người đi bộ nặng:",
"Đơn vị di chuyển chậm với lớp giáp được gia cố. Tuy tốc độ của chúng chậm,
"nhưng sức bền của chúng lại rất đáng nể."
	};

	// Text formatting
	float yPosition = 0.f;
	for (const auto& line : lines) {
		sf::Text text(line, mFont, 24);  // Adjust size as needed

		text.setPosition(20.f, yPosition);

		mInfoTexts.push_back(text);
		yPosition += text.getGlobalBounds().height + 10.f;
	}

	// Set up an green scroll area that matches the visible box
	mScrollArea.setSize(textAreaSize);
	mScrollArea.setPosition(textAreaPosition);
	mScrollArea.setFillColor(sf::Color(0, 255, 0, 100)); // Semi-transparent green
	mScrollArea.setOutlineColor(sf::Color::Green);
	mScrollArea.setOutlineThickness(2.f);

	// Max scroll (content height - viewport height)
	float contentHeight = mInfoTexts.back().getPosition().y +
		mInfoTexts.back().getGlobalBounds().height;
	mMaxOffset = std::max(0.f, contentHeight - textAreaSize.y);
}

void InformationState::draw()
{
	auto& window = *getContext().window;

	// Draw background in UI space
	window.setView(mUiView);
	window.draw(mBackgroundSprite);

	// Build a scroll view sized to the text box (in UI coordinates)
	const sf::Vector2f textAreaPos = mScrollArea.getPosition();
	const sf::Vector2f textAreaSize = mScrollArea.getSize();

	sf::View scrollView({ 0.f, 0.f, textAreaSize.x, textAreaSize.y });

	// Compute viewport inside the letterboxed UI viewport
	sf::FloatRect vp;
	vp.left = mUiViewport.left + (textAreaPos.x / 1920.f) * mUiViewport.width;
	vp.top = mUiViewport.top + (textAreaPos.y / 1080.f) * mUiViewport.height;
	vp.width = (textAreaSize.x / 1920.f) * mUiViewport.width;
	vp.height = (textAreaSize.y / 1080.f) * mUiViewport.height;
	scrollView.setViewport(vp);

	// Center view according to scroll offset (so (0,0) of content is at top-left)
	scrollView.setCenter(textAreaSize.x * 0.5f, mScrollOffset + textAreaSize.y * 0.5f);

	// Draw the scrollable text
	window.setView(scrollView);
	for (const auto& t : mInfoTexts) window.draw(t);

	// Back to UI view for buttons / overlays
	window.setView(mUiView);

	// Hover scale + draw close button
	sf::Vector2f mouseUi = window.mapPixelToCoords(sf::Mouse::getPosition(window), mUiView);
	if (mCloseInfoButton.getGlobalBounds().contains(mouseUi))
		mCloseInfoButton.setScale(1.2f, 1.2f);
	else
		mCloseInfoButton.setScale(1.1f, 1.1f);
	window.draw(mCloseInfoButton);
}

bool InformationState::update(sf::Time)
{
	return true;
}

bool InformationState::handleEvent(const sf::Event& event)
{
	auto& window = *getContext().window;

	// Keep the UI letterboxed when the window resizes
	if (event.type == sf::Event::Resized) {
		mUiViewport = makeLetterboxViewport({ event.size.width, event.size.height });
		mUiView.setViewport(mUiViewport);
	}

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mouseUi = window.mapPixelToCoords(
			{ event.mouseButton.x, event.mouseButton.y }, mUiView);

		if (mCloseInfoButton.getGlobalBounds().contains(mouseUi)) {
			requestStackPop();
			requestStackPush(States::Menu);
			return true;
		}
		// Start dragging if within the scroll area
		if (mScrollArea.getGlobalBounds().contains(mouseUi)) {
			mIsDragging = true;
			mLastMouseY = mouseUi.y;
		}
	}

	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
		mIsDragging = false;

	else if (event.type == sf::Event::MouseMoved && mIsDragging) {
		sf::Vector2f mouseUi = window.mapPixelToCoords(
			{ event.mouseMove.x, event.mouseMove.y }, mUiView);
		float dy = mLastMouseY - mouseUi.y;
		mScrollOffset = std::clamp(mScrollOffset + dy, 0.f, mMaxOffset);
		mLastMouseY = mouseUi.y;
	}

	else if (event.type == sf::Event::MouseWheelScrolled)
		mScrollOffset = std::clamp(mScrollOffset - event.mouseWheelScroll.delta * 40.f, 0.f, mMaxOffset);

	return true;
}

