#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <set>

struct ColorPair {
    int index;
    std::string majorColor;
    std::string minorColor;
};

class IColorMapPrinter {
public:
    virtual void print(const ColorPair& pair) = 0;
    virtual ~IColorMapPrinter() = default;
};

class MockPrinter : public IColorMapPrinter {
public:
    std::vector<ColorPair> capturedPairs;

    void print(const ColorPair& pair) override {
        capturedPairs.push_back(pair);
    }
};

class ColorMapGenerator {
public:
    std::vector<ColorPair> generate() {
        const char* majorColor[] = {"White", "Red", "Black", "Yellow", "Violet"};
        const char* minorColor[] = {"Blue", "Orange", "Green", "Brown", "Slate"};
        std::vector<ColorPair> colorMap;

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                colorMap.push_back({i * 5 + j, majorColor[i], minorColor[i]});
            }
        }
        return colorMap;
    }
};

int printColorMap(IColorMapPrinter& printer) {
    ColorMapGenerator generator;
    auto colorMap = generator.generate();
    for (const auto& pair : colorMap) {
        printer.print(pair);
    }
    return static_cast<int>(colorMap.size());
}

void testBugInColorMap() {
    MockPrinter mock;
    int result = printColorMap(mock);
    assert(result == 25);

    for (int i = 0; i < 5; ++i) {
        std::string expectedMajor = mock.capturedPairs[i * 5].majorColor;
        std::set<std::string> minorSet;
        for (int j = 0; j < 5; ++j) {
            const auto& pair = mock.capturedPairs[i * 5 + j];
            assert(pair.majorColor == expectedMajor);
            minorSet.insert(pair.minorColor);
        }
        assert(minorSet.size() == 5 && "Bug detected: minor colors are not varying!");
    }

    std::cout << "Bug test passed: minor colors vary correctly (if no bug)\n";
}
