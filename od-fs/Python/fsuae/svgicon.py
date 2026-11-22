import os
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from fsgui.image import Image


class SvgIcon:
    def __init__(self, name: str) -> None:
        self.name = name

    def get_image(
        self, size: int, tint_color: tuple[int, int, int, int] | None = None
    ) -> "Image | None":
        """Load the SVG as an Image at the specified size, optionally tinted with the given RGBA
        color."""
        try:
            from fsapp import fsapp
            from fsgui.image import Image

            # Find the SVG file in the resources directory
            image_path = os.path.join(fsapp.resources_dir, self.name)
            print(f"Checking SVG path: {image_path}")

            if os.path.exists(image_path):
                print(f"Found SVG file at: {image_path}")
                if image_path.lower().endswith(".svg"):
                    try:
                        print(f"Using sized SVG loader: {size}x{size}")
                        image = Image.from_svg(image_path, (size, size))
                        print("Sized SVG loader succeeded")
                    except Exception as e:
                        print(f"SVG loading failed: {e}, trying regular loader")
                        image = Image.from_file(image_path)
                        print("Fallback regular loader succeeded")
                else:
                    print(f"Not an SVG, using regular loader: {image_path}")
                    image = Image.from_file(image_path)
                    print("Regular loader succeeded")

                # Apply tint if specified
                if tint_color:
                    print(f"Applying tint color: {tint_color}")
                    image = image.tint(*tint_color)

                return image
            else:
                print(f"SVG file not found at: {image_path}")
                # Also check if resources_dir exists
                print(f"Resources directory: {fsapp.resources_dir}")
                print(f"Resources directory exists: {os.path.exists(fsapp.resources_dir)}")
                if os.path.exists(fsapp.resources_dir):
                    # List what's actually in the resources directory
                    try:
                        contents = os.listdir(fsapp.resources_dir)
                        print(f"Resources directory contents: {contents}")
                    except Exception as e:
                        print(f"Could not list resources directory: {e}")

            raise FileNotFoundError(f"SVG file not found: {self.name}")

        except Exception as e:
            print(f"Error loading SVG {self.name}: {e}")
            return None
