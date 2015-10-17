/* Copyright 2015 Henorvell Ge
 * 
 * This file is a part of CampusExpBot
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

class xml_helper
{
	public:
		// Initialization
		xml_helper(const char *file_path);
		~xml_helper();
	private:
		TiXmlDocument *doc;
};
