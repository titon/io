<?hh // strict
/**
 * @copyright   2010-2015, The Titon Project
 * @license     http://opensource.org/licenses/bsd-license.php
 * @link        http://titon.io
 */

namespace Titon\Io\Reader;

use Titon\Io\ResourceMap;
use Titon\Io\Exception\ReadErrorException;
use Titon\Utility\Col;
use RuntimeException;

/**
 * A reader that loads its configuration from an YAML file.
 * Requires the YAML extension to be installed.
 *
 * @package Titon\Io\Reader
 */
class YamlReader extends AbstractReader {

    /**
     * {@inheritdoc}
     */
    public function getResourceExt(): string {
        return 'yml';
    }

    /**
     * {@inheritdoc}
     *
     * @uses Titon\Utility\Col
     *
     * @throws \RuntimeException
     * @throws \Titon\Io\Exception\ReadErrorException
     */
    public function readResource(): ResourceMap {
        if (!extension_loaded('yaml')) {
            throw new RuntimeException('YAML extension must be enabled using `hhvm.enable_zend_compat = true`');
        }

        if ($this->exists()) {
            return Col::toMap(yaml_parse_file($this->getPath()))->toMap();
        }

        throw new ReadErrorException(sprintf('YamlReader failed to parse %s', $this->getPath()));
    }

}
