<?hh // strict
/**
 * @copyright   2010-2013, The Titon Project
 * @license     http://opensource.org/licenses/bsd-license.php
 * @link        http://titon.io
 */

namespace Titon\Io\Writer;

use Titon\Io\ResourceMap;
use Titon\Type\XmlDocument;

/**
 * A file writer that generates XML files.
 *
 * @package Titon\Io\Writer
 */
class XmlWriter extends AbstractWriter {

    /**
     * {@inheritdoc}
     *
     * @uses Titon\Type\XmlDocument
     */
    public function write(ResourceMap $data) {
        return parent::write(XmlDocument::fromMap($data)->toString());
    }

}