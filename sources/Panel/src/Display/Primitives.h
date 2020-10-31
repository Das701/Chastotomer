#pragma once
#include "Display/Colors.h"


namespace Display
{
    namespace Primitives
    {
        class Rectangle
        {
        public:
            Rectangle(int width, int height);
            void Draw(int x, int y, Color color = Color::Number);
            void Fill(int x, int y, Color color = Color::Number);
        private:
            int width;
            int height;
        };


        class Point
        {
        public:
            Point() {};
            void Draw(int x, int y, Color color = Color::Number);
        };


        class HLine
        {
        public:
            HLine(int _length) : length(_length) {};
            void Draw(int x, int y, Color color = Color::Number);
        private:
            const int length;
        };


        class VLine
        {
        public:
            VLine(int _length) : length(_length) {};
            void Draw(int x, int y, Color color = Color::Number);
        private:
            const int length;
        };
    }
}
