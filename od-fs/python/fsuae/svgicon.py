import os
from typing import Optional


class SvgIcon:
    def __init__(self, name: str) -> None:
        self.name = name

    def get_image(
        self, size: int, tint_color: tuple[int, int, int, int] = None
    ) -> Optional["Image"]:
        """Load the SVG as an Image at the specified size, optionally tinted with the given RGBA color."""
        try:
            from fsgui.image import Image
            from fsapp import fsapp
            import fsgui_image  # type: ignore

            # Find the SVG file in the resources directory
            image_path = os.path.join(fsapp.resources_dir, self.name)
            print(f"Checking SVG path: {image_path}")

            if os.path.exists(image_path):
                print(f"Found SVG file at: {image_path}")
                # Use sized SVG loader if available, otherwise fall back to regular loader
                if image_path.lower().endswith(".svg"):
                    try:
                        if hasattr(fsgui_image, "load_sized_svg"):
                            print(f"Using sized SVG loader: {size}x{size}")
                            image_handle = fsgui_image.load_sized_svg(image_path, size, size)
                            print(f"Sized SVG loader succeeded")
                        else:
                            print("Sized SVG loader not available, using regular loader")
                            image_handle = fsgui_image.load(image_path)
                            print(f"Regular loader succeeded")
                    except Exception as e:
                        print(f"SVG loading failed: {e}, trying regular loader")
                        image_handle = fsgui_image.load(image_path)
                        print(f"Fallback regular loader succeeded")
                else:
                    print(f"Not an SVG, using regular loader: {image_path}")
                    image_handle = fsgui_image.load(image_path)
                    print(f"Regular loader succeeded")

                print(f"Image handle created: {image_handle}")
                image = Image(image_handle)

                # Apply tint if specified
                if tint_color:
                    print(f"Applying tint color: {tint_color}")
                    image.tint(*tint_color)

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
