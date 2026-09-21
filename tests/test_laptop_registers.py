import importlib.util
from pathlib import Path
import unittest

path = Path(__file__).resolve().parents[1] / 'python/02_laptop_modbus_slave/laptop_slave_array_sender.py'
spec = importlib.util.spec_from_file_location('laptop_slave', path)
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


class RegisterTests(unittest.TestCase):
    def test_initial_layout(self):
        self.assertEqual(module.register_values(1), [1, 11, 21, 31, 1])

    def test_rollover_never_exceeds_register_capacity(self):
        self.assertEqual(module.register_values(65535), [65535, 9, 19, 29, 65535])
        for counter in (0, 65505, 65525, 65535, 65536):
            self.assertTrue(all(0 <= n <= 65535 for n in module.register_values(counter)))

    def test_real_datastore_round_trip(self):
        store = module.ModbusSlaveContext(hr=module.ModbusSequentialDataBlock(0, [0] * 5), zero_mode=True)
        values = module.register_values(65535)
        store.setValues(3, 0, values)
        self.assertEqual(store.getValues(3, 0, 5), values)


if __name__ == '__main__':
    unittest.main()
