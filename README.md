# LowPoly

LowPoly on Android. Convert a picture into LowPoly style.

## Usage

Two ways:

1. LowPoly.getLowPolyBitmap(Bitmap bitmap, int accuracy, int randomPointCount);

	This will Convert a bitmap into LowPoly style bitmap directly.
	
2. LowPoly.lowPoly(int[] pixels, int width, int height, int accuracy, int pointCount);

	This will generate Delaunay Triangles from the given bitmap`s pixels. Then you can use these triangles to draw on canvas.

## Shot

![](./screenshots/sample.gif)

## Licence

	Copyright 2017 Fichardu
	
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at
	
	    http://www.apache.org/licenses/LICENSE-2.0
	
	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.