from fsuae.servicecontainer import ServiceContainer


class AmigaContext:
    # FIXME: Disallow looking up via instance later! Instance must be propgated instead.
    _instance: "AmigaContext"

    @classmethod
    def instance(cls) -> "AmigaContext":
        return cls._instance

    def __init__(self, services: ServiceContainer):
        # Actual config
        self.config = services.uae_config.config2

        # Actual ports - and what devices are actually inserted into ports
        self.ports = services.input_ports.ports
